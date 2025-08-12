#include "ModbusDevice.h"
#include "Logger.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QDebug>

ModbusDevice::ModbusDevice(const QString& name, QObject* parent)
    : QObject(parent)
    , deviceName(name)
    , connection(std::make_unique<ModbusConnection>())
    , isDeviceConnected(false)  // connected yerine isDeviceConnected
    , pollingTimer(nullptr)
    , watchdogTimer(nullptr)
    , polling(false)
    , pollingInterval(1000)
    , watchdogInterval(5000)
    , totalRequests(0)
    , successfulRequests(0)
    , failedRequests(0)
{
    setupTimers();

    // Bağlantıları kur
    QObject::connect(connection.get(), &ModbusConnection::deviceConnected,  // connected yerine deviceConnected
            this, &ModbusDevice::deviceConnected);  // connected yerine deviceConnected
    QObject::connect(connection.get(), &ModbusConnection::deviceDisconnected,  // disconnected yerine deviceDisconnected
            this, &ModbusDevice::deviceDisconnected);  // disconnected yerine deviceDisconnected
    QObject::connect(connection.get(), &ModbusConnection::connectionError,
            this, &ModbusDevice::onConnectionError);
    QObject::connect(connection.get(), &ModbusConnection::communicationError,
            this, &ModbusDevice::onCommunicationError);
    QObject::connect(connection.get(), &ModbusConnection::requestCompleted,
            this, &ModbusDevice::onRequestCompleted);
}

ModbusDevice::~ModbusDevice()
{
    stopPolling();
    disconnectDevice();  // disconnect yerine disconnectDevice
    cleanupTimers();
}

void ModbusDevice::setupTimers()
{
    pollingTimer = new QTimer(this);
    watchdogTimer = new QTimer(this);

    pollingTimer->setInterval(pollingInterval);
    watchdogTimer->setInterval(watchdogInterval);

    QObject::connect(pollingTimer, &QTimer::timeout,
            this, &ModbusDevice::handlePollingTimeout);
    QObject::connect(watchdogTimer, &QTimer::timeout,
            this, &ModbusDevice::handleWatchdogTimeout);
}

void ModbusDevice::cleanupTimers()
{
    if (pollingTimer) {
        pollingTimer->stop();
        delete pollingTimer;
        pollingTimer = nullptr;
    }

    if (watchdogTimer) {
        watchdogTimer->stop();
        delete watchdogTimer;
        watchdogTimer = nullptr;
    }
}

bool ModbusDevice::connectToDevice()  // connect yerine connectToDevice
{
    if (isDeviceConnected) {  // connected yerine isDeviceConnected
        return true;
    }

    if (!connection->connectDevice(connectionParams)) {
        lastError = connection->getLastError();
        return false;
    }

    isDeviceConnected = true;  // connected yerine isDeviceConnected
    lastCommunicationTime = QDateTime::currentDateTime();

    if (polling) {
        startPolling();
    }

    return true;
}

void ModbusDevice::disconnectDevice()  // disconnect yerine disconnectDevice
{
    stopPolling();
    
    if (isDeviceConnected) {  // connected yerine isDeviceConnected
        connection->disconnectDevice();  // disconnect yerine disconnectDevice
        isDeviceConnected = false;  // connected yerine isDeviceConnected
        lastError.clear();
    }
}

bool ModbusDevice::isConnected() const
{
    return isDeviceConnected && connection->isConnected();  // connected yerine isDeviceConnected
}

bool ModbusDevice::reconnect()
{
    disconnectDevice();  // disconnect yerine disconnectDevice
    return connectToDevice();  // connect yerine connectToDevice
}

void ModbusDevice::setConnectionParams(const ModbusTypes::ConnectionParams& params)
{
    bool wasConnected = isDeviceConnected;  // connected yerine isDeviceConnected
    bool wasPolling = polling;
    
    if (wasConnected) {
        disconnectDevice();  // disconnect yerine disconnectDevice
    }
    
    connectionParams = params;
    
    if (wasConnected) {
        connectToDevice();  // connect yerine connectToDevice
        if (wasPolling) {
            startPolling();
        }
    }
    
    emit configurationChanged();
}

ModbusTypes::ConnectionParams ModbusDevice::getConnectionParams() const
{
    return connectionParams;
}

void ModbusDevice::setPollingInterval(int ms)
{
    if (ms > 0 && ms != pollingInterval) {
        pollingInterval = ms;
        if (pollingTimer) {
            pollingTimer->setInterval(ms);
        }
        emit configurationChanged();
    }
}

int ModbusDevice::getPollingInterval() const
{
    return pollingInterval;
}

bool ModbusDevice::addRegister(const ModbusTypes::RegisterConfig& config)
{
    QMutexLocker locker(&registerMutex);
    
    if (!validateRegisterConfig(config)) {
        lastError = "Invalid register configuration";
        return false;
    }
    
    if (registers.contains(config.address)) {
        lastError = "Register already exists";
        return false;
    }
    
    auto reg = std::make_shared<ModbusRegister>(config);
    registers[config.address] = reg;
    
    QObject::connect(reg.get(), &ModbusRegister::valueChanged,  // connect yerine QObject::connect
            this, [this, address = config.address](const QVariant& value) {
                emit registerValueChanged(address, value);
            });
    
    emit registerAdded(config.address);
    emit configurationChanged();
    
    return true;
}

bool ModbusDevice::removeRegister(int address)
{
    QMutexLocker locker(&registerMutex);
    
    if (!registers.contains(address)) {
        return false;
    }
    
    registers.remove(address);
    emit registerRemoved(address);
    emit configurationChanged();
    
    return true;
}

bool ModbusDevice::updateRegister(int address, const ModbusTypes::RegisterConfig& config)
{
    QMutexLocker locker(&registerMutex);
    
    if (!registers.contains(address)) {
        lastError = "Register does not exist";
        return false;
    }
    
    if (!validateRegisterConfig(config)) {
        lastError = "Invalid register configuration";
        return false;
    }
    
    registers[address]->updateConfig(config);
    emit registerUpdated(address);
    emit configurationChanged();
    
    return true;
}

ModbusTypes::RegisterConfig ModbusDevice::getRegisterConfig(int address) const
{
    QMutexLocker locker(const_cast<QMutex*>(&registerMutex));  // const_cast ekledik
    
    if (registers.contains(address)) {
        return registers[address]->getConfig();
    }
    
    return ModbusTypes::RegisterConfig();
}

QList<int> ModbusDevice::getRegisterAddresses() const
{
    QMutexLocker locker(const_cast<QMutex*>(&registerMutex));  // const_cast ekledik
    return registers.keys();
}

QVariant ModbusDevice::getRegisterValue(int address) const
{
    QMutexLocker locker(const_cast<QMutex*>(&registerMutex));  // const_cast ekledik
    
    if (registers.contains(address)) {
        return registers[address]->getValue();
    }
    
    return QVariant();
}

bool ModbusDevice::setRegisterValue(int address, const QVariant& value)
{
    QMutexLocker locker(&registerMutex);
    
    if (!registers.contains(address)) {
        lastError = "Register does not exist";
        return false;
    }
    
    auto reg = registers[address];
    if (reg->isReadOnly()) {
        lastError = "Register is read-only";
        return false;
    }
    
    return reg->setValue(value);
}

void ModbusDevice::startPolling()
{
    if (!polling && isDeviceConnected) {  // connected yerine isDeviceConnected
        polling = true;
        pollingTimer->start();
        watchdogTimer->start();
        emit pollingStarted();
    }
}

void ModbusDevice::stopPolling()
{
    if (polling) {
        polling = false;
        pollingTimer->stop();
        watchdogTimer->stop();
        emit pollingStopped();
    }
}

bool ModbusDevice::isPolling() const
{
    return polling;
}

void ModbusDevice::handlePollingTimeout()
{
    if (!isDeviceConnected || registers.isEmpty()) {  // connected yerine isDeviceConnected
        return;
    }
    
    QList<ModbusTypes::RegisterConfig> requests;
    {
        QMutexLocker locker(&registerMutex);
        for (const auto& reg : registers) {
            requests.append(reg->getConfig());
        }
    }
    
    if (optimizeRegisterRequests(requests)) {
        processRegisterUpdates();
    }
}

void ModbusDevice::handleWatchdogTimeout()
{
    if (!isDeviceConnected) {  // connected yerine isDeviceConnected
        return;
    }
    
    QDateTime now = QDateTime::currentDateTime();
    if (lastCommunicationTime.msecsTo(now) > watchdogInterval) {
        handleCommunicationTimeout();
    }
}

void ModbusDevice::onConnectionError(const QString& error)
{
    lastError = error;
    emit connectionError(error);
}

void ModbusDevice::onCommunicationError(const QString& error)
{
    lastError = error;
    emit communicationError(error);
}

void ModbusDevice::onRegisterValueChanged(int address, const QVariant& value)
{
    lastCommunicationTime = QDateTime::currentDateTime();
    emit registerValueChanged(address, value);
}

void ModbusDevice::onRequestCompleted(bool success)
{
    updateStatistics(success, 0.0);
}

void ModbusDevice::updateStatistics(bool success, double responseTime)
{
    totalRequests++;
    if (success) {
        successfulRequests++;
    } else {
        failedRequests++;
    }
    
    if (responseTime > 0) {
        responseTimes.append(responseTime);
        if (responseTimes.size() > 1000) {  // Son 1000 yanıt süresini tut
            responseTimes.removeFirst();
        }
    }
    
    emit statisticsUpdated();
}

int ModbusDevice::getTotalRequests() const
{
    return totalRequests;
}

int ModbusDevice::getSuccessfulRequests() const
{
    return successfulRequests;
}

int ModbusDevice::getFailedRequests() const
{
    return failedRequests;
}

double ModbusDevice::getAverageResponseTime() const
{
    if (responseTimes.isEmpty()) {
        return 0.0;
    }
    
    double sum = 0.0;
    for (double time : responseTimes) {
        sum += time;
    }
    return sum / responseTimes.size();
}

void ModbusDevice::resetStatistics()
{
    totalRequests = 0;
    successfulRequests = 0;
    failedRequests = 0;
    responseTimes.clear();
    emit statisticsUpdated();
}

QString ModbusDevice::getLastError() const
{
    return lastError;
}

void ModbusDevice::clearError()
{
    lastError.clear();
}

QDateTime ModbusDevice::getLastCommunicationTime() const
{
    return lastCommunicationTime;
}

int ModbusDevice::getRetryCount() const
{
    return connectionParams.retryCount;
}

void ModbusDevice::setRetryCount(int count)
{
    if (count >= 0 && count != connectionParams.retryCount) {
        connectionParams.retryCount = count;
        emit configurationChanged();
    }
}

bool ModbusDevice::validateRegisterConfig(const ModbusTypes::RegisterConfig& config) const
{
    // Adres kontrolü
    if (config.address < 0 || config.address > 65535) {
        return false;
    }
    
    // Veri tipi kontrolü
    switch (config.dataType) {
        case ModbusTypes::DataType::BIT:
        case ModbusTypes::DataType::BYTE:
        case ModbusTypes::DataType::WORD:
        case ModbusTypes::DataType::INT:
        case ModbusTypes::DataType::DWORD:
        case ModbusTypes::DataType::DINT:
        case ModbusTypes::DataType::REAL:
        case ModbusTypes::DataType::LREAL:
        case ModbusTypes::DataType::STRING:
        case ModbusTypes::DataType::WSTRING:
            break;
        default:
            return false;
    }
    
    // Register tipi kontrolü
    switch (config.regType) {
        case ModbusTypes::RegisterType::DISCRETE_INPUT:
        case ModbusTypes::RegisterType::COIL:
        case ModbusTypes::RegisterType::INPUT_REGISTER:
        case ModbusTypes::RegisterType::HOLDING_REGISTER:
            break;
        default:
            return false;
    }
    
    // Ölçekleme faktörü kontrolü
    if (config.scaleFactor == 0.0) {
        return false;
    }
    
    // Min/max değer kontrolü
    if (config.minValue >= config.maxValue) {
        return false;
    }
    
    // Alarm limitleri kontrolü
    if (config.isAlarmEnabled) {
        if (config.alarmLowLimit >= config.alarmHighLimit) {
            return false;
        }
        if (config.alarmLowLimit < config.minValue || 
            config.alarmHighLimit > config.maxValue) {
            return false;
        }
    }
    
    return true;
}

bool ModbusDevice::optimizeRegisterRequests(QList<ModbusTypes::RegisterConfig>& requests) const
{
    if (requests.isEmpty()) {
        return false;
    }
    
    // Register'ları tipine göre grupla
    QMap<ModbusTypes::RegisterType, QList<ModbusTypes::RegisterConfig>> groupedRequests;
    for (const auto& req : requests) {
        groupedRequests[req.regType].append(req);
    }
    
    // Her grup için register'ları sırala ve birleştir
    for (auto& group : groupedRequests) {
        std::sort(group.begin(), group.end(), 
                 [](const ModbusTypes::RegisterConfig& a, const ModbusTypes::RegisterConfig& b) {
                     return a.address < b.address;
                 });
        
        // Ardışık register'ları birleştir
        QList<ModbusTypes::RegisterConfig> optimized;
        ModbusTypes::RegisterConfig current = group.first();
        // Quantity alanı yok, bu yüzden yeni bir değişken kullanıyoruz
        int currentQuantity = 1;
        
        for (int i = 1; i < group.size(); i++) {
            const auto& next = group[i];
            
            // Register'lar ardışık ve aynı veri tipinde mi?
            if (next.address == current.address + currentQuantity && 
                next.dataType == current.dataType) {
                // Birleştir
                currentQuantity++;
            } else {
                // Yeni grup başlat
                // Quantity'yi bir şekilde taşımamız gerekiyor
                // Burada bir workaround kullanabiliriz
                optimized.append(current);
                current = next;
                currentQuantity = 1;
            }
        }
        optimized.append(current);
        
        group = optimized;
    }
    
    // Optimize edilmiş requestleri birleştir
    requests.clear();
    for (const auto& group : groupedRequests) {
        requests.append(group);
    }
    
    return true;
}

void ModbusDevice::processRegisterUpdates()
{
    QMutexLocker locker(&registerMutex);
    
    for (auto& reg : registers) {
        if (!reg->isValid()) {
            continue;
        }
        
        const auto& config = reg->getConfig();
        
        // Register tipine göre okuma işlemi yap
        switch (config.regType) {
            case ModbusTypes::RegisterType::DISCRETE_INPUT:
            {
                uint8_t value;
                if (connection->readDiscreteInputs(config.address, 1, &value)) {
                    reg->setValue(value != 0);
                }
                break;
            }
            case ModbusTypes::RegisterType::COIL:
            {
                uint8_t value;
                if (connection->readCoils(config.address, 1, &value)) {
                    reg->setValue(value != 0);
                }
                break;
            }
            case ModbusTypes::RegisterType::INPUT_REGISTER:
            {
                uint16_t value;
                if (connection->readInputRegisters(config.address, 1, &value)) {
                    reg->setValue(value);
                }
                break;
            }
            case ModbusTypes::RegisterType::HOLDING_REGISTER:
            {
                uint16_t value;
                if (connection->readHoldingRegisters(config.address, 1, &value)) {
                    reg->setValue(value);
                }
                break;
            }
        }
    }
}

void ModbusDevice::handleCommunicationTimeout()
{
    logDebug("Communication timeout detected");
    reconnect();
}

void ModbusDevice::logDebug(const QString& message) const
{
    qDebug() << "ModbusDevice[" << deviceName << "]:" << message;
}

bool ModbusDevice::saveConfiguration(const QString& filename) const
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly)) {
        const_cast<ModbusDevice*>(this)->lastError = "Failed to open configuration file for writing";  // const_cast ekledik
        return false;
    }

    QJsonObject json = QJsonObject::fromVariantMap(configurationToVariantMap());
    QJsonDocument doc(json);
    
    if (file.write(doc.toJson()) == -1) {
        const_cast<ModbusDevice*>(this)->lastError = "Failed to write configuration data";  // const_cast ekledik
        return false;
    }
    
    return true;
}

bool ModbusDevice::loadConfiguration(const QString& filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        lastError = "Failed to open configuration file for reading";
        return false;
    }

    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    
    if (doc.isNull() || !doc.isObject()) {
        lastError = "Invalid configuration file format";
        return false;
    }

    bool wasConnected = isDeviceConnected;  // connected yerine isDeviceConnected
    bool wasPolling = polling;
    
    if (wasConnected) {
        disconnectDevice();  // disconnect yerine disconnectDevice
    }

    if (!configurationFromVariantMap(doc.object().toVariantMap())) {
        if (wasConnected) {
            connectToDevice();  // connect yerine connectToDevice
            if (wasPolling) {
                startPolling();
            }
        }
        return false;
    }

    if (wasConnected) {
        connectToDevice();  // connect yerine connectToDevice
        if (wasPolling) {
            startPolling();
        }
    }

    emit configurationChanged();
    return true;
}

QVariantMap ModbusDevice::configurationToVariantMap() const
{
    QVariantMap map;
    
    // Temel özellikler
    map["deviceName"] = deviceName;
    map["pollingInterval"] = pollingInterval;
    map["watchdogInterval"] = watchdogInterval;
    
    // Bağlantı parametreleri
    QVariantMap connParams;
    connParams["name"] = connectionParams.name;
    connParams["ip"] = connectionParams.ip;
    connParams["port"] = connectionParams.port;
    connParams["slaveId"] = connectionParams.slaveId;
    connParams["timeout"] = connectionParams.timeout;
    connParams["scanRate"] = connectionParams.scanRate;
    connParams["retryCount"] = connectionParams.retryCount;
    connParams["type"] = static_cast<int>(connectionParams.type);
    connParams["byteOrder"] = static_cast<int>(connectionParams.byteOrder);
    connParams["readWriteEnabled"] = connectionParams.readWriteEnabled;
    connParams["errorRecovery"] = static_cast<int>(connectionParams.errorRecovery);
    connParams["serialPort"] = connectionParams.serialPort;
    connParams["baudRate"] = connectionParams.baudRate;
    connParams["parity"] = QString(connectionParams.parity);
    connParams["dataBits"] = connectionParams.dataBits;
    connParams["stopBits"] = connectionParams.stopBits;
    map["connectionParams"] = connParams;
    
    // Register yapılandırmaları
    QVariantList registerList;
    QMutexLocker locker(const_cast<QMutex*>(&registerMutex));  // const_cast ekledik
    for (const auto& reg : registers) {
        QVariantMap regMap;
        const auto& config = reg->getConfig();
        
        regMap["address"] = config.address;
        regMap["name"] = config.name;
        regMap["description"] = config.description;
        regMap["dataType"] = static_cast<int>(config.dataType);
        regMap["regType"] = static_cast<int>(config.regType);
        regMap["scaleFactor"] = config.scaleFactor;
        regMap["unit"] = config.unit;
        regMap["minValue"] = config.minValue;
        regMap["maxValue"] = config.maxValue;
        regMap["isReadOnly"] = config.isReadOnly;
        regMap["isAlarmEnabled"] = config.isAlarmEnabled;
        regMap["alarmHighLimit"] = config.alarmHighLimit;
        regMap["alarmLowLimit"] = config.alarmLowLimit;
        regMap["byteOrder"] = static_cast<int>(config.byteOrder);
        
        registerList.append(regMap);
    }
    map["registers"] = registerList;
    
    return map;
}

bool ModbusDevice::configurationFromVariantMap(const QVariantMap& map)
{
    // Temel özellikler
    deviceName = map["deviceName"].toString();
    pollingInterval = map["pollingInterval"].toInt();
    watchdogInterval = map["watchdogInterval"].toInt();
    
    // Bağlantı parametreleri
    QVariantMap connParams = map["connectionParams"].toMap();
    connectionParams.name = connParams["name"].toString();
    connectionParams.ip = connParams["ip"].toString();
    connectionParams.port = connParams["port"].toInt();
    connectionParams.slaveId = connParams["slaveId"].toInt();
    connectionParams.timeout = connParams["timeout"].toInt();
    connectionParams.scanRate = connParams["scanRate"].toInt();
    connectionParams.retryCount = connParams["retryCount"].toInt();
    connectionParams.type = static_cast<ModbusTypes::ConnectionType>(connParams["type"].toInt());
    connectionParams.byteOrder = static_cast<ModbusTypes::ByteOrder>(connParams["byteOrder"].toInt());
    connectionParams.readWriteEnabled = connParams["readWriteEnabled"].toBool();
    connectionParams.errorRecovery = static_cast<ModbusTypes::ErrorRecoveryMode>(connParams["errorRecovery"].toInt());
    connectionParams.serialPort = connParams["serialPort"].toString();
    connectionParams.baudRate = connParams["baudRate"].toInt();
    connectionParams.parity = connParams["parity"].toString()[0].toLatin1();
    connectionParams.dataBits = connParams["dataBits"].toInt();
    connectionParams.stopBits = connParams["stopBits"].toInt();
    
    // Register yapılandırmaları
    QVariantList registerList = map["registers"].toList();
    QMutexLocker locker(&registerMutex);
    registers.clear();
    
    for (const QVariant& v : registerList) {
        QVariantMap regMap = v.toMap();
        ModbusTypes::RegisterConfig config;
        
        config.address = regMap["address"].toInt();
        config.name = regMap["name"].toString();
        config.description = regMap["description"].toString();
        config.dataType = static_cast<ModbusTypes::DataType>(regMap["dataType"].toInt());
        config.regType = static_cast<ModbusTypes::RegisterType>(regMap["regType"].toInt());
        config.scaleFactor = regMap["scaleFactor"].toDouble();
        config.unit = regMap["unit"].toString();
        config.minValue = regMap["minValue"].toDouble();
        config.maxValue = regMap["maxValue"].toDouble();
        config.isReadOnly = regMap["isReadOnly"].toBool();
        config.isAlarmEnabled = regMap["isAlarmEnabled"].toBool();
        config.alarmHighLimit = regMap["alarmHighLimit"].toDouble();
        config.alarmLowLimit = regMap["alarmLowLimit"].toDouble();
        config.byteOrder = static_cast<ModbusTypes::ByteOrder>(regMap["byteOrder"].toInt());
        
        if (validateRegisterConfig(config)) {
            auto reg = std::make_shared<ModbusRegister>(config);
            registers[config.address] = reg;
            
            QObject::connect(reg.get(), &ModbusRegister::valueChanged,  // connect yerine QObject::connect
                    this, [this, address = config.address](const QVariant& value) {
                        emit registerValueChanged(address, value);
                    });
        }
    }
    
    return true;
}

void ModbusDevice::timerEvent(QTimerEvent* event)
{
    Q_UNUSED(event);
}