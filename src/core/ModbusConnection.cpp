#include "ModbusConnection.h"
#include <QDebug>
#include <QSerialPort>
#include <QHostAddress>
#include <QTimerEvent>
#include <QElapsedTimer>
#include <errno.h>

#ifdef Q_OS_WIN
#include <winsock2.h>
#endif

ModbusConnection::ModbusConnection(QObject* parent)
    : QObject(parent)
    , ctx(nullptr)
    , m_isConnected(false)
    , queueTimer(new QTimer(this))
    , watchdogTimer(new QTimer(this))
    , queueProcessingInterval(10)
    , watchdogInterval(5000)
    , totalRequests(0)
    , successfulRequests(0)
    , failedRequests(0)
{
    // Timer ayarlarını yap
    queueTimer->setInterval(queueProcessingInterval);
    watchdogTimer->setInterval(watchdogInterval);

    // Timer bağlantılarını kur
    connect(queueTimer, &QTimer::timeout, this, &ModbusConnection::processQueue);
    connect(watchdogTimer, &QTimer::timeout, this, &ModbusConnection::checkConnection);
}

ModbusConnection::~ModbusConnection()
{
    disconnectDevice();
    
    if (queueTimer) {
        queueTimer->stop();
        delete queueTimer;
        queueTimer = nullptr;
    }
    
    if (watchdogTimer) {
        watchdogTimer->stop();
        delete watchdogTimer;
        watchdogTimer = nullptr;
    }
}

int ModbusConnection::setupModbusPort()
{
    if (setupConnection()) {
        return 0;
    }
    return -1;
}

bool ModbusConnection::connectDevice(const ModbusTypes::ConnectionParams& params)
{
    if (m_isConnected) {
        disconnectDevice();
    }
    
    this->params = params;
    
    if (!setupConnection()) {
        lastError = tr("Failed to setup connection");
        emit connectionError(lastError);
        return false;
    }
    
    m_isConnected = true;
    lastCommunicationTime = QDateTime::currentDateTime();
    
    queueTimer->start();
    watchdogTimer->start();
    
    emit deviceConnected();
    logDebug("Connected successfully");
    
    return true;
}

void ModbusConnection::disconnectDevice()
{
    queueTimer->stop();
    watchdogTimer->stop();
    
    QMutexLocker locker(&queueMutex);
    requestQueue.clear();
    
    if (m_isConnected) {
        cleanupConnection();
        m_isConnected = false;
        emit deviceDisconnected();
        logDebug("Disconnected");
    }
}

bool ModbusConnection::reconnect()
{
    disconnectDevice();
    return connectDevice(params);
}

bool ModbusConnection::setupConnection()
{
    cleanupConnection();
    
    switch (params.type) {
        case ModbusTypes::ConnectionType::TCP_IP:
            return setupTcpConnection();
        case ModbusTypes::ConnectionType::UDP_IP:
            return setupUdpConnection();
        case ModbusTypes::ConnectionType::RTU_OVER_TCP:
            return setupRtuOverTcpConnection();
        case ModbusTypes::ConnectionType::RTU_OVER_UDP:
            return setupRtuOverUdpConnection();
        case ModbusTypes::ConnectionType::RTU_SERIAL:
        case ModbusTypes::ConnectionType::ASCII_SERIAL:
            return setupSerialConnection();
    }
    
    return false;
}

bool ModbusConnection::setupTcpConnection()
{
    ctx = modbus_new_tcp(params.ip.toLatin1().constData(), params.port);
    if (ctx == nullptr) {
        lastError = tr("Failed to create TCP context: %1").arg(modbus_strerror(errno));
        return false;
    }
    
    if (modbus_connect(ctx) == -1) {
        lastError = tr("TCP connection failed: %1").arg(modbus_strerror(errno));
        modbus_free(ctx);
        ctx = nullptr;
        return false;
    }
    
    return true;
}

bool ModbusConnection::setupUdpConnection()
{
    ctx = modbus_new_tcp(params.ip.toLatin1().constData(), params.port);
    if (ctx == nullptr) {
        lastError = tr("Failed to create UDP context: %1").arg(modbus_strerror(errno));
        return false;
    }
    
    if (modbus_connect(ctx) == -1) {
        lastError = tr("UDP connection failed: %1").arg(modbus_strerror(errno));
        modbus_free(ctx);
        ctx = nullptr;
        return false;
    }
    
    return true;
}

bool ModbusConnection::setupRtuOverTcpConnection()
{
    ctx = modbus_new_tcp(params.ip.toLatin1().constData(), params.port);
    if (ctx == nullptr) {
        lastError = tr("Failed to create RTU over TCP context: %1").arg(modbus_strerror(errno));
        return false;
    }
    
    if (modbus_connect(ctx) == -1) {
        lastError = tr("RTU over TCP connection failed: %1").arg(modbus_strerror(errno));
        modbus_free(ctx);
        ctx = nullptr;
        return false;
    }
    
    return true;
}

bool ModbusConnection::setupRtuOverUdpConnection()
{
    ctx = modbus_new_tcp(params.ip.toLatin1().constData(), params.port);
    if (ctx == nullptr) {
        lastError = tr("Failed to create RTU over UDP context: %1").arg(modbus_strerror(errno));
        return false;
    }
    
    if (modbus_connect(ctx) == -1) {
        lastError = tr("RTU over UDP connection failed: %1").arg(modbus_strerror(errno));
        modbus_free(ctx);
        ctx = nullptr;
        return false;
    }
    
    return true;
}

bool ModbusConnection::setupSerialConnection()
{
#ifdef MODBUS_RTU_ENABLED
    ctx = modbus_new_rtu(params.serialPort.toLatin1().constData(),
                        params.baudRate,
                        params.parity,
                        params.dataBits,
                        params.stopBits);
#else
    ctx = modbus_new_tcp(params.ip.toLatin1().constData(), params.port);
#endif
    
    if (ctx == nullptr) {
        lastError = tr("Failed to create serial context: %1").arg(modbus_strerror(errno));
        return false;
    }
    
    if (!configureSerialPort()) {
        lastError = tr("Failed to configure serial port");
        modbus_free(ctx);
        ctx = nullptr;
        return false;
    }
    
    if (modbus_connect(ctx) == -1) {
        lastError = tr("Serial connection failed: %1").arg(modbus_strerror(errno));
        modbus_free(ctx);
        ctx = nullptr;
        return false;
    }
    
    return true;
}

void ModbusConnection::cleanupConnection()
{
    if (ctx != nullptr) {
        modbus_close(ctx);
        modbus_free(ctx);
        ctx = nullptr;
    }
}

bool ModbusConnection::validateConnection() const
{
    return (ctx != nullptr && m_isConnected);
}

bool ModbusConnection::configureSerialPort()
{
    if (!ctx) return false;
    
#ifdef MODBUS_RTU_ENABLED
    if (params.type == ModbusTypes::ConnectionType::RTU_SERIAL ||
        params.type == ModbusTypes::ConnectionType::ASCII_SERIAL) {
        // Serial port özel ayarları burada yapılabilir
        return true;
    }
#endif
    
    return true;
}

bool ModbusConnection::configureTimeouts()
{
    if (!ctx) return false;
    
    uint32_t sec = params.timeout / 1000;
    uint32_t usec = (params.timeout % 1000) * 1000;
    
    if (modbus_set_response_timeout(ctx, sec, usec) == -1) {
        return false;
    }
    
    if (modbus_set_byte_timeout(ctx, sec, usec) == -1) {
        return false;
    }
    
    return true;
}

bool ModbusConnection::configureSlaveId()
{
    if (!ctx) return false;
    return (modbus_set_slave(ctx, params.slaveId) != -1);
}

void ModbusConnection::setResponseTimeout(uint32_t ms)
{
    if (ctx) {
        uint32_t sec = ms / 1000;
        uint32_t usec = (ms % 1000) * 1000;
        modbus_set_response_timeout(ctx, sec, usec);
    }
}

void ModbusConnection::setByteTimeout(uint32_t ms)
{
    if (ctx) {
        uint32_t sec = ms / 1000;
        uint32_t usec = (ms % 1000) * 1000;
        modbus_set_byte_timeout(ctx, sec, usec);
    }
}

void ModbusConnection::setDebugMode(bool enabled)
{
    if (ctx) {
        modbus_set_debug(ctx, enabled ? TRUE : FALSE);
    }
}

void ModbusConnection::setErrorRecoveryMode(ModbusTypes::ErrorRecoveryMode mode)
{
    if (!ctx) return;

    modbus_error_recovery_mode modeValue;
    switch (mode) {
        case ModbusTypes::ErrorRecoveryMode::NONE:
            modeValue = MODBUS_ERROR_RECOVERY_NONE;
            break;
        case ModbusTypes::ErrorRecoveryMode::LINK:
            modeValue = MODBUS_ERROR_RECOVERY_LINK;
            break;
        case ModbusTypes::ErrorRecoveryMode::PROTOCOL:
            modeValue = MODBUS_ERROR_RECOVERY_PROTOCOL;
            break;
        case ModbusTypes::ErrorRecoveryMode::LINK_AND_PROTOCOL:
            modeValue = (modbus_error_recovery_mode)(MODBUS_ERROR_RECOVERY_LINK | MODBUS_ERROR_RECOVERY_PROTOCOL);
            break;
        default:
            modeValue = MODBUS_ERROR_RECOVERY_NONE;
    }
    modbus_set_error_recovery(ctx, modeValue);
}

void ModbusConnection::processQueue()
{
    QMutexLocker locker(&queueMutex);
    
    if (requestQueue.isEmpty() || !m_isConnected) {
        return;
    }
    
    ModbusRequest request = requestQueue.dequeue();
    locker.unlock();  // Mutex'i serbest bırak
    
    double responseTime = 0.0;
    bool success = false;
    
    try {
        QElapsedTimer timer;
        timer.start();
        
        success = executeRequest(request);
        responseTime = timer.elapsed();
        
        updateStatistics(success, responseTime);
        emit requestCompleted(success);
    }
    catch (const std::exception& e) {
        qDebug() << "Error in processQueue:" << e.what();
        emit communicationError(tr("Error processing request: %1").arg(e.what()));
    }
}

bool ModbusConnection::executeRequest(const ModbusRequest& request)
{
    if (!validateRequest(request)) {
        return false;
    }
    
    int result = -1;
    
    switch (request.type) {
        case ModbusTypes::RegisterType::COIL:
            if (request.isWrite) {
                if (request.quantity == 1) {
                    result = modbus_write_bit(ctx, request.address, 
                            request.writeData[0] != 0);
                } else {
                    result = modbus_write_bits(ctx, request.address,
                            request.quantity, (uint8_t*)request.writeData.constData());
                }
            } else {
                uint8_t* buffer = new uint8_t[request.quantity];
                result = modbus_read_bits(ctx, request.address, request.quantity, buffer);
                if (result > 0) {
                    // Buffer'ı işle
                }
                delete[] buffer;
            }
            break;
            
        case ModbusTypes::RegisterType::DISCRETE_INPUT:
            if (!request.isWrite) {
                uint8_t* buffer = new uint8_t[request.quantity];
                result = modbus_read_input_bits(ctx, request.address, 
                        request.quantity, buffer);
                if (result > 0) {
                    // Buffer'ı işle
                }
                delete[] buffer;
            }
            break;
            
        case ModbusTypes::RegisterType::HOLDING_REGISTER:
            if (request.isWrite) {
                if (request.quantity == 1) {
                    result = modbus_write_register(ctx, request.address,
                            *(uint16_t*)request.writeData.constData());
                } else {
                    result = modbus_write_registers(ctx, request.address,
                            request.quantity, (uint16_t*)request.writeData.constData());
                }
            } else {
                uint16_t* buffer = new uint16_t[request.quantity];
                result = modbus_read_registers(ctx, request.address, 
                        request.quantity, buffer);
                if (result > 0) {
                    // Buffer'ı işle
                }
                delete[] buffer;
            }
            break;
            
        case ModbusTypes::RegisterType::INPUT_REGISTER:
            if (!request.isWrite) {
                uint16_t* buffer = new uint16_t[request.quantity];
                result = modbus_read_input_registers(ctx, request.address, 
                        request.quantity, buffer);
                if (result > 0) {
                    // Buffer'ı işle
                }
                delete[] buffer;
            }
            break;
    }
    
    return processResponse(result, request);
}

bool ModbusConnection::validateRequest(const ModbusRequest& request) const
{
    if (!validateConnection()) {
        return false;
    }
    
    return validateAddress(request.address, request.quantity, request.type);
}

bool ModbusConnection::validateAddress(int addr, int quantity, ModbusTypes::RegisterType type) const
{
    if (addr < 0 || quantity <= 0) {
        return false;
    }
    
    int maxAddr = addr + quantity - 1;
    int limit;
    
    switch (type) {
        case ModbusTypes::RegisterType::COIL:
        case ModbusTypes::RegisterType::DISCRETE_INPUT:
            limit = MODBUS_MAX_READ_BITS;
            break;
        case ModbusTypes::RegisterType::HOLDING_REGISTER:
        case ModbusTypes::RegisterType::INPUT_REGISTER:
            limit = MODBUS_MAX_READ_REGISTERS;
            break;
        default:
            return false;
    }
    
    return maxAddr <= limit;
}

bool ModbusConnection::processResponse(int result, const ModbusRequest& request)
{
    if (result == -1) {
        lastError = formatModbusError(errno);
        emit communicationError(lastError);
        return false;
    }
    
    lastCommunicationTime = QDateTime::currentDateTime();
    return true;
}

void ModbusConnection::updateStatistics(bool success, double responseTime)
{
    totalRequests++;
    if (success) {
        successfulRequests++;
    } else {
        failedRequests++;
    }
    
    responseTimes.append(responseTime);
    if (responseTimes.size() > 1000) {  // Son 1000 yanıt süresini tut
        responseTimes.removeFirst();
    }
    
    emit statisticsUpdated();
}

void ModbusConnection::resetStatistics()
{
    totalRequests = 0;
    successfulRequests = 0;
    failedRequests = 0;
    responseTimes.clear();
    emit statisticsUpdated();
}

void ModbusConnection::checkConnection()
{
    if (!m_isConnected) return;
    
    if (lastCommunicationTime.msecsTo(QDateTime::currentDateTime()) > 
            params.timeout * params.retryCount) {
        logDebug("Connection timeout detected");
        reconnect();
    }
}

void ModbusConnection::handleTimeout()
{
    if (m_isConnected) {
        lastError = tr("Communication timeout");
        emit communicationError(lastError);
        reconnect();
    }
}

void ModbusConnection::enqueueRequest(const ModbusRequest& request)
{
    QMutexLocker locker(&queueMutex);
    requestQueue.enqueue(request);
}

QString ModbusConnection::formatModbusError(int errorCode) const
{
    switch (errorCode) {
        case EMBXILFUN: return tr("Illegal function");
        case EMBXILADD: return tr("Illegal data address");
        case EMBXILVAL: return tr("Illegal data value");
        case EMBXSFAIL: return tr("Slave device failure");
        case EMBXACK: return tr("Acknowledge");
        case EMBXSBUSY: return tr("Slave device busy");
        case EMBXNACK: return tr("Negative acknowledge");
        case EMBXMEMPAR: return tr("Memory parity error");
        case EMBXGPATH: return tr("Gateway path unavailable");
        case EMBXGTAR: return tr("Gateway target device failed to respond");
        default: return tr("Unknown error: %1").arg(errorCode);
    }
}

void ModbusConnection::logDebug(const QString& message) const
{
    qDebug() << "ModbusConnection:" << message;
    emit const_cast<ModbusConnection*>(this)->debugMessage(message);
}

void ModbusConnection::timerEvent(QTimerEvent* event)
{
    Q_UNUSED(event);
}

double ModbusConnection::getAverageResponseTime() const
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

bool ModbusConnection::readCoils(int addr, int nb, uint8_t* dest)
{
    ModbusRequest request;
    request.address = addr;
    request.quantity = nb;
    request.type = ModbusTypes::RegisterType::COIL;
    request.isWrite = false;
    request.timestamp = QDateTime::currentDateTime();
    
    enqueueRequest(request);
    return true;
}

bool ModbusConnection::readDiscreteInputs(int addr, int nb, uint8_t* dest)
{
    ModbusRequest request;
    request.address = addr;
    request.quantity = nb;
    request.type = ModbusTypes::RegisterType::DISCRETE_INPUT;
    request.isWrite = false;
    request.timestamp = QDateTime::currentDateTime();
    
    enqueueRequest(request);
    return true;
}

bool ModbusConnection::readHoldingRegisters(int addr, int nb, uint16_t* dest)
{
    ModbusRequest request;
    request.address = addr;
    request.quantity = nb;
    request.type = ModbusTypes::RegisterType::HOLDING_REGISTER;
    request.isWrite = false;
    request.timestamp = QDateTime::currentDateTime();
    
    enqueueRequest(request);
    return true;
}

bool ModbusConnection::readInputRegisters(int addr, int nb, uint16_t* dest)
{
    ModbusRequest request;
    request.address = addr;
    request.quantity = nb;
    request.type = ModbusTypes::RegisterType::INPUT_REGISTER;
    request.isWrite = false;
    request.timestamp = QDateTime::currentDateTime();
    
    enqueueRequest(request);
    return true;
}

bool ModbusConnection::writeCoil(int addr, int status)
{
    ModbusRequest request;
    request.address = addr;
    request.quantity = 1;
    request.type = ModbusTypes::RegisterType::COIL;
    request.isWrite = true;
    request.writeData.append(status ? 1 : 0);
    request.timestamp = QDateTime::currentDateTime();
    
    enqueueRequest(request);
    return true;
}

bool ModbusConnection::writeRegister(int addr, int value)
{
    ModbusRequest request;
    request.address = addr;
    request.quantity = 1;
    request.type = ModbusTypes::RegisterType::HOLDING_REGISTER;
    request.isWrite = true;
    request.writeData.resize(2);
    *reinterpret_cast<uint16_t*>(request.writeData.data()) = value;
    request.timestamp = QDateTime::currentDateTime();
    
    enqueueRequest(request);
    return true;
}

bool ModbusConnection::writeMultipleCoils(int addr, int nb, const uint8_t* src)
{
    ModbusRequest request;
    request.address = addr;
    request.quantity = nb;
    request.type = ModbusTypes::RegisterType::COIL;
    request.isWrite = true;
    request.writeData = QByteArray(reinterpret_cast<const char*>(src), (nb + 7) / 8);
    request.timestamp = QDateTime::currentDateTime();
    
    enqueueRequest(request);
    return true;
}

bool ModbusConnection::writeMultipleRegisters(int addr, int nb, const uint16_t* src)
{
    ModbusRequest request;
    request.address = addr;
    request.quantity = nb;
    request.type = ModbusTypes::RegisterType::HOLDING_REGISTER;
    request.isWrite = true;
    request.writeData = QByteArray(reinterpret_cast<const char*>(src), nb * 2);
    request.timestamp = QDateTime::currentDateTime();
    
    enqueueRequest(request);
    return true;
}

bool ModbusConnection::readWriteMultipleRegisters(int read_addr, int read_nb, uint16_t* dest,
                                                int write_addr, int write_nb, const uint16_t* src)
{
    ModbusRequest request;
    request.address = read_addr;
    request.quantity = read_nb;
    request.type = ModbusTypes::RegisterType::HOLDING_REGISTER;
    request.isWrite = true;
    
    QByteArray data;
    data.append(reinterpret_cast<const char*>(&write_addr), sizeof(write_addr));
    data.append(reinterpret_cast<const char*>(&write_nb), sizeof(write_nb));
    data.append(reinterpret_cast<const char*>(src), write_nb * 2);
    request.writeData = data;
    request.timestamp = QDateTime::currentDateTime();
    
    enqueueRequest(request);
    return true;
}

bool ModbusConnection::maskWriteRegister(int addr, uint16_t and_mask, uint16_t or_mask)
{
    ModbusRequest request;
    request.address = addr;
    request.quantity = 1;
    request.type = ModbusTypes::RegisterType::HOLDING_REGISTER;
    request.isWrite = true;
    
    QByteArray data;
    data.append(reinterpret_cast<const char*>(&and_mask), sizeof(and_mask));
    data.append(reinterpret_cast<const char*>(&or_mask), sizeof(or_mask));
    request.writeData = data;
    request.timestamp = QDateTime::currentDateTime();
    
    enqueueRequest(request);
    return true;
}