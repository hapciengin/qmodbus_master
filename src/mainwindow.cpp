#include "mainwindow.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QApplication>
#include "DeviceConfigDialog.h"
#include <QThread>
#include "RegisterTableModel.h"
#include <QStyledItemDelegate>
#include <QComboBox>
#include <errno.h>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	pollTimer(nullptr),
	isPolling(false),
	pollRate(1000),
	currentDeviceId(0),
	dataLogger(nullptr)
{
    ui->setupUi(this);
    setupConnections();
    setupTable();
	setupPolling(); 

	dataLogger = new DataLogger();
	dataLogger->show();
    
    loadSettings();
}

MainWindow::~MainWindow()
{
    stopPolling();
    saveSettings();
	
	if (dataLogger) {
        delete dataLogger;
    }
    
    for(auto ctx : modbusContexts) {
        if(ctx) {
            modbus_close(ctx);
            modbus_free(ctx);
        }
    }
    delete ui;
}

void MainWindow::setupPolling()
{
    if (!pollTimer) {
        pollTimer = new QTimer(this);
        connect(pollTimer, &QTimer::timeout, this, &MainWindow::updateRegisters);
        pollTimer->setInterval(pollRate);
    }
}

void MainWindow::updateRegisterValue(int row, uint16_t value)
{
    if (row < 0 || row >= ui->registerTable->rowCount()) return;
    
    QTableWidgetItem* item = ui->registerTable->item(row, 1);
    if (!item) {
        item = new QTableWidgetItem();
        ui->registerTable->setItem(row, 1, item);
    }
    
    auto &config = deviceConfigs[currentDeviceId];
    QString formattedValue = formatRegisterValue(value, 
                                               config.registerFormats[row],
                                               0);
    item->setText(formattedValue);
}

void MainWindow::processReadResults(int deviceId, uint16_t* registers, int count)
{
    auto &config = deviceConfigs[deviceId];
    
    for(int i = 0; i < count; i++) {
        qDebug() << "Register" << i << "raw value:" << registers[i];
        
        // Tabloyu güncelle
        QTableWidgetItem* item = ui->registerTable->item(i, 1);
        if (!item) {
            item = new QTableWidgetItem();
            ui->registerTable->setItem(i, 1, item);
        }
        
        QString value = formatRegisterValue(registers[i], 
                                         config.registerFormats[i],
                                         i < count-1 ? registers[i+1] : 0);
                                         
        qDebug() << "Register" << i << "formatted value:" << value;
        item->setText(value);
        
        // DataLogger'a veri gönder
        if (dataLogger) {
            int address = config.startAddress + i;
            QString unit = "";
            double scaledValue = registers[i];
            
            if (config.registers.contains(address)) {
                const auto& regConfig = config.registers[address];
                unit = regConfig.unit;
                scaledValue = registers[i] * regConfig.scaleFactor;
            }
            
            dataLogger->logData(address, QVariant(scaledValue), unit);
        }
    }
}

QString MainWindow::getRegisterTypeString(const QString& plcType) const
{
    if (plcType == "Input Register") return "3x";
    if (plcType == "Holding Register") return "4x";
    if (plcType == "Discrete Input") return "1x";
    if (plcType == "Coil") return "0x";
    return "";
}

int MainWindow::adjustRegisterAddress(int address, const QString& plcType) const
{
    auto& config = deviceConfigs[currentDeviceId];
    
    // PLC tipine göre adres dönüşümü
    if (plcType.contains("Modicon")) {
        return address - 40001;
    }
    else if (plcType.contains("Delta")) {
        return address - 2000;
    }
    else if (plcType.contains("Siemens")) {
        return address;
    }
    else if (plcType.contains("Allen Bradley")) {
        return address;
    }
    
    return address - config.addressOffset;
}

void MainWindow::setupConnections()
{
    connect(ui->connectButton, &QPushButton::clicked, this, &MainWindow::onConnectButtonClicked);
    connect(ui->startPollButton, &QPushButton::clicked, this, &MainWindow::onStartPollClicked);
    connect(ui->pollRateSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onPollRateChanged(int)));
    connect(ui->registerTable, &QTableWidget::cellChanged, this, &MainWindow::onRegisterValueChanged);
    connect(ui->addDeviceButton, &QPushButton::clicked, this, &MainWindow::onAddDeviceClicked);
    connect(ui->removeDeviceButton, &QPushButton::clicked, this, &MainWindow::onRemoveDeviceClicked);
    connect(ui->saveButton, &QPushButton::clicked, this, &MainWindow::onSaveConfigClicked);
    connect(ui->loadButton, &QPushButton::clicked, this, &MainWindow::onLoadConfigClicked);
    connect(ui->deviceComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onDeviceSelectionChanged(int)));
    connect(ui->setupButton, &QPushButton::clicked, this, &MainWindow::onRegisterSetupClicked);
}

class ComboBoxDelegate : public QStyledItemDelegate {
public:
    ComboBoxDelegate(QObject* parent = nullptr) : QStyledItemDelegate(parent) {}

    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option,
                        const QModelIndex& index) const override {
        if (index.column() == 2) {
            QComboBox* editor = new QComboBox(parent);
            editor->addItems(RegisterTableModel::getAvailableFormats());
            return editor;
        }
        return QStyledItemDelegate::createEditor(parent, option, index);
    }

    void setEditorData(QWidget* editor, const QModelIndex& index) const override {
        if (QComboBox* comboBox = qobject_cast<QComboBox*>(editor)) {
            QString currentText = index.data(Qt::EditRole).toString();
            int idx = comboBox->findText(currentText);
            if (idx >= 0) {
                comboBox->setCurrentIndex(idx);
            }
        } else {
            QStyledItemDelegate::setEditorData(editor, index);
        }
    }

    void setModelData(QWidget* editor, QAbstractItemModel* model,
                     const QModelIndex& index) const override {
        if (QComboBox* comboBox = qobject_cast<QComboBox*>(editor)) {
            model->setData(index, comboBox->currentText(), Qt::EditRole);
        } else {
            QStyledItemDelegate::setModelData(editor, model, index);
        }
    }
};

void MainWindow::setupTable()
{
    ui->registerTable->setColumnCount(4);
    QStringList headers;
    headers << "Address" << "Value" << "Format" << "Description";
    ui->registerTable->setHorizontalHeaderLabels(headers);

    ui->registerTable->setItemDelegateForColumn(2, new ComboBoxDelegate(ui->registerTable));
}

void MainWindow::onConnectButtonClicked()
{
    if(!deviceConfigs.contains(currentDeviceId)) {
        QMessageBox::warning(this, "Warning", "Please select or add a device first.");
        return;
    }
    
    if(!modbusContexts.contains(currentDeviceId)) {
        connectDevice(currentDeviceId);
    } else {
        disconnectDevice(currentDeviceId);
    }
}

void MainWindow::connectDevice(int deviceId)
{
    auto &config = deviceConfigs[deviceId];
    
    // Önceki bağlantıyı temizle
    if(modbusContexts.contains(deviceId)) {
        disconnectDevice(deviceId);
        QThread::msleep(100); // Bağlantının kapanması için bekle
    }
    
    qDebug() << "Connecting to device" << deviceId << "at" << config.ip << ":" << config.port;
    
    modbus_t *ctx = modbus_new_tcp(config.ip.toLatin1().constData(), config.port);
    if(ctx == NULL) {
        QString errorMsg = QString("Failed to create modbus context: %1").arg(modbus_strerror(errno));
        qDebug() << errorMsg;
        updateStatus(errorMsg);
        return;
    }
    
    // Debug modu
    modbus_set_debug(ctx, TRUE);
    
    // Yazma izinlerini aktifleştir
	modbus_set_error_recovery(ctx, static_cast<modbus_error_recovery_mode>(MODBUS_ERROR_RECOVERY_LINK | MODBUS_ERROR_RECOVERY_PROTOCOL));
    
    // Timeout ayarları
    struct timeval timeout;
    timeout.tv_sec = 2;  // 2 saniye
    timeout.tv_usec = 0;
    
    if (modbus_set_response_timeout(ctx, timeout.tv_sec, timeout.tv_usec) == -1) {
        qDebug() << "Failed to set response timeout:" << modbus_strerror(errno);
    }
    
    if (modbus_set_byte_timeout(ctx, timeout.tv_sec, timeout.tv_usec) == -1) {
        qDebug() << "Failed to set byte timeout:" << modbus_strerror(errno);
    }
    
    // Slave ID ayarla
    if (modbus_set_slave(ctx, config.slaveId) == -1) {
        qDebug() << "Failed to set slave ID:" << modbus_strerror(errno);
    }
    
    qDebug() << "Attempting to connect...";
    
    // Bağlantı denemesi öncesi kısa bekleme
    QThread::msleep(100);
    
    // Bağlantı denemesi
    int retryCount = 0;
    bool connected = false;
    
    while (retryCount < config.retryCount && !connected) {
        if (modbus_connect(ctx) == -1) {
            QString errorMsg = QString("Connection attempt %1 failed: %2")
                             .arg(retryCount + 1)
                             .arg(modbus_strerror(errno));
            qDebug() << errorMsg;
            
            if (retryCount < config.retryCount - 1) {
                QThread::msleep(1000); // Yeniden denemeden önce 1 saniye bekle
                retryCount++;
            } else {
                updateStatus("Connection failed: " + QString(modbus_strerror(errno)));
                modbus_free(ctx);
                return;
            }
        } else {
            connected = true;
        }
    }
    
    // Bağlantı başarılı
    modbusContexts[deviceId] = ctx;
    config.isActive = true;
    
    // TCP tamponunu temizle
    modbus_flush(ctx);
    
    // Bağlantı testi yap
    uint16_t testReg;
    int testAddr = adjustRegisterAddress(config.startAddress, config.plcType);
    if (modbus_read_registers(ctx, testAddr, 1, &testReg) == -1) {
        qDebug() << "Warning: Initial read test failed:" << modbus_strerror(errno);
    } else {
        qDebug() << "Initial read test successful";
    }
    
    ui->connectButton->setText("Disconnect");
    updateStatus(QString("Connected to: %1:%2 (Slave ID: %3)")
                .arg(config.ip)
                .arg(config.port)
                .arg(config.slaveId));
    
    // Bağlantı başarılı olduktan sonra register tablosunu güncelle
    setupRegisterTable(config);
    
    qDebug() << "Connection established successfully";
    qDebug() << "Device configuration:";
    qDebug() << "  IP:" << config.ip;
    qDebug() << "  Port:" << config.port;
    qDebug() << "  Slave ID:" << config.slaveId;
    qDebug() << "  PLC Type:" << config.plcType;
    qDebug() << "  Start Address:" << config.startAddress;
    qDebug() << "  Quantity:" << config.quantity;
    qDebug() << "  Address Offset:" << config.addressOffset;
}

void MainWindow::disconnectDevice(int deviceId)
{
    if(modbusContexts.contains(deviceId)) {
        modbus_t* ctx = modbusContexts[deviceId];
        if(ctx) {
            modbus_close(ctx);
            modbus_free(ctx);
        }
        modbusContexts.remove(deviceId);
        
        deviceConfigs[deviceId].isActive = false;
        ui->connectButton->setText("Connect");
        updateStatus("Disconnected");
        
        // Bağlantının tamamen kapanması için kısa bir bekleme
        QThread::msleep(100);
    }
}

void MainWindow::onStartPollClicked()
{
    if (!isPolling) {
        startPolling();
        ui->startPollButton->setText("Stop Polling");
    } else {
        stopPolling();
        ui->startPollButton->setText("Start Polling");
    }
}

void MainWindow::startPolling()
{
    if (modbusContexts.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Please connect to a device first.");
        return;
    }
    
    isPolling = true;
    pollTimer->start(pollRate);
}

void MainWindow::stopPolling()
{
    isPolling = false;
    pollTimer->stop();
}

void MainWindow::onPollRateChanged(int value)
{
    pollRate = value;
    if (isPolling) {
        pollTimer->setInterval(pollRate);
    }
}


QString MainWindow::formatRegisterValue(uint16_t value, const QString &format, uint16_t nextValue)
{
    auto &config = deviceConfigs[currentDeviceId];
    
    if (format == "Dec") {
        if (config.dataType == "INT16") {
            return QString::number(static_cast<int16_t>(value));
        } else if (config.dataType == "UINT16") {
            return QString::number(value);
        } else if (config.dataType == "INT32 (Double Int)") {
            int32_t int32Value = (static_cast<int32_t>(value) << 16) | nextValue;
            return QString::number(int32Value);
        } else if (config.dataType == "UINT32 (Double Word)") {
            uint32_t uint32Value = (static_cast<uint32_t>(value) << 16) | nextValue;
            return QString::number(uint32Value);
        } else if (config.dataType == "FLOAT32") {
            uint16_t regs[2] = {value, nextValue};
            float fval = 0.0;
            
            // Byte sıralamasına göre uygun fonksiyonu kullan
            if (config.byteOrder == "AB CD (Big-Endian)") {
                fval = modbus_get_float(regs);
            } else if (config.byteOrder == "CD AB (Little-Endian)") {
                uint16_t swapped[2] = {nextValue, value};
                fval = modbus_get_float(swapped);
            } else if (config.byteOrder == "BA DC (Big-Endian Byte Swap)") {
                uint16_t swapped[2] = {
                    static_cast<uint16_t>(((value & 0xFF) << 8) | ((value & 0xFF00) >> 8)),
                    static_cast<uint16_t>(((nextValue & 0xFF) << 8) | ((nextValue & 0xFF00) >> 8))
                };
                fval = modbus_get_float(swapped);
            } else {
                fval = modbus_get_float_dcba(regs);
            }
            
            return QString::number(fval, 'f', 2);
        } else if (config.dataType == "BOOL") {
            return value ? "1" : "0";
        } else {
            // Varsayılan olarak normal değeri döndür
            return QString::number(value);
        }
    } else if (format == "Hex") {
        return QString("0x%1").arg(value, 4, 16, QChar('0'));
    } else if (format == "Bin") {
        return QString("0b%1").arg(value, 16, 2, QChar('0'));
    } else if (format == "ASCII") {
        QString result;
        char c1 = (value >> 8) & 0xFF;
        char c2 = value & 0xFF;
        
        if (c1 >= 32 && c1 <= 126) result += c1;
        else result += '.';
        
        if (c2 >= 32 && c2 <= 126) result += c2;
        else result += '.';
        
        return result;
    } else if (format == "FLOAT") {
        float fval = *reinterpret_cast<float*>(&value);
        return QString::number(fval, 'f', 3);
    } else if (format == "DOUBLE") {
        double dval = *reinterpret_cast<double*>(&value);
        return QString::number(dval, 'f', 6);
    } else if (format == "WORD") {
        return QString("0x%1").arg(value, 4, 16, QChar('0'));
    } else if (format == "DWORD") {
        uint32_t dwordValue = (static_cast<uint32_t>(value) << 16) | nextValue;
        return QString("0x%1").arg(dwordValue, 8, 16, QChar('0'));
    }
    
    // Varsayılan olarak normal değeri döndür
    return QString::number(value);
}

void MainWindow::onRegisterValueChanged(int row, int column)
{
    if (column == 1 && modbusContexts.contains(currentDeviceId)) {
        // Değer değişikliği
        bool ok;
        int value = ui->registerTable->item(row, column)->text().toInt(&ok);
        if (ok) {
            auto &config = deviceConfigs[currentDeviceId];
            writeRegister(currentDeviceId, config.startAddress + row, value);

            // DataLogger'a veriyi gönder
            if (dataLogger) {
                QString unit = "";
                int address = config.startAddress + row;
                
                // Register konfigürasyonundan birimi al
                if (config.registers.contains(address)) {
                    unit = config.registers[address].unit;
                }
                
                // Ham değeri ve ölçeklenmiş değeri hesapla
                double scaledValue = value;
                if (config.registers.contains(address)) {
                    scaledValue = value * config.registers[address].scaleFactor;
                }
                
                dataLogger->logData(address, QVariant(scaledValue), unit);
            }
        }
    }
    else if (column == 2) {
        // Format değişikliği
        QString newFormat = ui->registerTable->item(row, 2)->text();
        auto &config = deviceConfigs[currentDeviceId];
        
        // Format listesini güncelle
        if (row < config.registerFormats.size()) {
            config.registerFormats[row] = newFormat;
        } else {
            while (config.registerFormats.size() <= row) {
                config.registerFormats.append("Dec");
            }
            config.registerFormats[row] = newFormat;
        }
        
        // Değeri yeniden formatla
        if (modbusContexts.contains(currentDeviceId)) {
            updateRegisters();
        }
    }
}

void MainWindow::writeRegister(int deviceId, int addr, uint16_t value)
{
    if(!modbusContexts.contains(deviceId)) return;
    
    auto &config = deviceConfigs[deviceId];
    
    // PLC tipine göre adres düzeltmesi yap
    int adjustedAddr = adjustRegisterAddress(addr, config.plcType);
    
    qDebug() << "Writing to device" << deviceId;
    qDebug() << "Original address:" << addr;
    qDebug() << "Adjusted address:" << adjustedAddr;
    qDebug() << "Value:" << value;
    
    if(modbus_write_register(modbusContexts[deviceId], adjustedAddr, value) == -1) {
        QString errorMsg = QString("Write error: %1").arg(modbus_strerror(errno));
        qDebug() << errorMsg;
        updateStatus(errorMsg);
        
        // Hata durumunda tekrar deneme
        for(int retry = 0; retry < config.retryCount; retry++) {
            qDebug() << "Retrying write operation, attempt" << (retry + 1);
            QThread::msleep(100); // Kısa bir bekleme
            
            if(modbus_write_register(modbusContexts[deviceId], adjustedAddr, value) != -1) {
                qDebug() << "Write successful on retry" << (retry + 1);
                updateStatus("Write successful");
                return;
            }
        }
        
        handleCommunicationError(deviceId, errorMsg);
    } else {
        qDebug() << "Write successful";
        updateStatus("Write successful");
    }
}


void MainWindow::onConnectionError()
{
    QMessageBox::critical(this, tr("Connection Error"), tr("Failed to connect to the device"));
    updateStatus("Connection failed");
}

void MainWindow::onAddDeviceClicked()
{
    if(deviceConfigs.size() >= 50) {
        QMessageBox::warning(this, "Warning", "Maximum device limit (50) reached!");
        return;
    }

    DeviceConfig config;
    // Varsayılan değerleri ayarla
    config.ip = "127.0.0.1";
    config.port = 502;
    config.slaveId = 1;
    config.startAddress = 40001;  // Değişiklik burada
    config.quantity = 10;
    config.plcType = "Holding Register"; // Siemens için varsayılan tip
    
    DeviceConfigDialog dialog(config, this);
    
    if(dialog.exec()) {
        int newId = deviceConfigs.isEmpty() ? 1 : deviceConfigs.lastKey() + 1;
        deviceConfigs[newId] = config;
        updateDeviceList();
        
        // Yeni cihazı seç
        int index = ui->deviceComboBox->findText(QString("%1 (%2)").arg(config.ip).arg(newId));
        if (index >= 0) {
            ui->deviceComboBox->setCurrentIndex(index);
        }
    }
}

void MainWindow::onRemoveDeviceClicked()
{
    if (deviceConfigs.isEmpty()) {
        QMessageBox::warning(this, "Warning", "No device to remove.");
        return;
    }

    // Seçili cihazı bul
    int index = ui->deviceComboBox->currentIndex();
    if (index < 0) {
        QMessageBox::warning(this, "Warning", "Please select a device to remove.");
        return;
    }

    QString text = ui->deviceComboBox->currentText();
    // IP adresini ve ID'yi ayır (örnek format: "127.0.0.1 (1)")
    int startPos = text.lastIndexOf('(');
    int endPos = text.lastIndexOf(')');
    
    if (startPos != -1 && endPos != -1) {
        QString idStr = text.mid(startPos + 1, endPos - startPos - 1);
        bool ok;
        int deviceId = idStr.toInt(&ok);
        
        if (ok) {
            qDebug() << "Removing device with ID:" << deviceId;

            // Eğer cihaz bağlıysa önce bağlantıyı kes
            if (modbusContexts.contains(deviceId)) {
                disconnectDevice(deviceId);
            }

            // Cihazı konfigürasyondan sil
            deviceConfigs.remove(deviceId);

            // UI'ı güncelle
            updateDeviceList();

            // Status'u güncelle
            updateStatus("Device removed successfully");

            // Eğer başka cihaz kalmadıysa tabloyu temizle
            if (deviceConfigs.isEmpty()) {
                ui->registerTable->setRowCount(0);
            }
        }
    }
}

void MainWindow::updateDeviceList()
{
    ui->deviceComboBox->clear();
    for(auto it = deviceConfigs.begin(); it != deviceConfigs.end(); ++it) {
        ui->deviceComboBox->addItem(QString("%1 (%2)").arg(it.value().ip).arg(it.key()));
    }
}

QVariant MainWindow::deviceConfigToVariant(const DeviceConfig &config)
{
    QVariantMap map;
    map["ip"] = config.ip;
    map["port"] = config.port;
    map["slaveId"] = config.slaveId;
    map["startAddress"] = config.startAddress;
    map["quantity"] = config.quantity;
    map["registerFormats"] = QVariant::fromValue(config.registerFormats);
    map["isActive"] = config.isActive;
    map["deviceName"] = config.deviceName;
    map["plcType"] = config.plcType;
    map["ipConfig"] = config.ipConfig;
    map["timeout"] = config.timeout;
    map["retryCount"] = config.retryCount;
    
    // Register yapılandırmalarını kaydet
    QVariantMap registersMap;
    for(auto it = config.registers.begin(); it != config.registers.end(); ++it) {
        QVariantMap regMap;
        regMap["address"] = it.value().address;
        regMap["name"] = it.value().name;
        regMap["dataType"] = it.value().dataType;
        regMap["scaleFactor"] = it.value().scaleFactor;
        regMap["unit"] = it.value().unit;
        regMap["minValue"] = it.value().minValue;
        regMap["maxValue"] = it.value().maxValue;
        registersMap[QString::number(it.key())] = regMap;
    }
    map["registers"] = registersMap;
    
    return map;
}

DeviceConfig MainWindow::variantToDeviceConfig(const QVariant &variant)
{
    QVariantMap map = variant.toMap();
    DeviceConfig config;
    
    config.ip = map["ip"].toString();
    config.port = map["port"].toInt();
    config.slaveId = map["slaveId"].toInt();
    config.startAddress = map["startAddress"].toInt();
    config.quantity = map["quantity"].toInt();
    config.registerFormats = map["registerFormats"].toStringList();
    config.isActive = map["isActive"].toBool();
    config.deviceName = map["deviceName"].toString();
    config.plcType = map["plcType"].toString();
    config.ipConfig = map["ipConfig"].toString();
    config.timeout = map["timeout"].toInt();
    config.retryCount = map["retryCount"].toInt();
    
    // Register yapılandırmalarını yükle
    QVariantMap registersMap = map["registers"].toMap();
    for(auto it = registersMap.begin(); it != registersMap.end(); ++it) {
        QVariantMap regMap = it.value().toMap();
        RegisterConfig regConfig;
        regConfig.address = regMap["address"].toInt();
        regConfig.name = regMap["name"].toString();
        regConfig.dataType = regMap["dataType"].toString();
        regConfig.scaleFactor = regMap["scaleFactor"].toDouble();
        regConfig.unit = regMap["unit"].toString();
        regConfig.minValue = regMap["minValue"].toDouble();
        regConfig.maxValue = regMap["maxValue"].toDouble();
        config.registers[it.key().toInt()] = regConfig;
    }
    
    return config;
}

void MainWindow::onSaveConfigClicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Save Configuration"), "",
        tr("Configuration Files (*.json);;All Files (*)"));
        
    if (fileName.isEmpty()) return;
    
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this, tr("Error"),
                           tr("Cannot write file %1:\n%2.")
                           .arg(QDir::toNativeSeparators(fileName),
                                file.errorString()));
        return;
    }
    
    QJsonObject json;
    for(auto it = deviceConfigs.begin(); it != deviceConfigs.end(); ++it) {
        json[QString::number(it.key())] = QJsonValue::fromVariant(deviceConfigToVariant(it.value()));
    }
    
    QJsonDocument doc(json);
    file.write(doc.toJson());
    
    updateStatus("Configuration saved to " + fileName);
}

void MainWindow::onLoadConfigClicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Load Configuration"), "",
        tr("Configuration Files (*.json);;All Files (*)"));
        
    if (fileName.isEmpty()) return;
    
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, tr("Error"),
                           tr("Cannot read file %1:\n%2.")
                           .arg(QDir::toNativeSeparators(fileName),
                                file.errorString()));
        return;
    }
    
    QByteArray data = file.readAll();
    QJsonDocument doc(QJsonDocument::fromJson(data));
    QJsonObject json = doc.object();
    
    deviceConfigs.clear();
    for(auto it = json.begin(); it != json.end(); ++it) {
        deviceConfigs[it.key().toInt()] = variantToDeviceConfig(it.value().toVariant());
    }
    
    updateDeviceList();
    updateStatus("Configuration loaded from " + fileName);
}

void MainWindow::onDeviceSelectionChanged(int index)
{
    qDebug() << "=== onDeviceSelectionChanged ===";
    qDebug() << "Index:" << index;
    
    if(index < 0) {
        qDebug() << "Invalid index";
        return;
    }
    
    QString text = ui->deviceComboBox->itemText(index);
    qDebug() << "ComboBox text:" << text;
    
    int startPos = text.lastIndexOf('(');
    int endPos = text.lastIndexOf(')');
    
    qDebug() << "Parsing positions - start:" << startPos << "end:" << endPos;
    
    if (startPos != -1 && endPos != -1 && startPos < endPos) {
        QString idStr = text.mid(startPos + 1, endPos - startPos - 1);
        qDebug() << "ID string:" << idStr;
        
        bool ok;
        int deviceId = idStr.toInt(&ok);
        
        if (ok) {
            qDebug() << "Device ID:" << deviceId;
            currentDeviceId = deviceId;
            
            if(deviceConfigs.contains(currentDeviceId)) {
                const DeviceConfig& config = deviceConfigs[currentDeviceId];
                qDebug() << "Config found:";
                qDebug() << "  Quantity:" << config.quantity;
                qDebug() << "  Formats:" << config.registerFormats;
                qDebug() << "  Start Address:" << config.startAddress;
                setupRegisterTable(config);
            } else {
                qDebug() << "No config found for device ID:" << deviceId;
            }
        } else {
            qDebug() << "Failed to convert ID string to integer:" << idStr;
        }
    } else {
        qDebug() << "Failed to parse device ID from text";
    }
    qDebug() << "=== End onDeviceSelectionChanged ===";
}

// src/mainwindow.cpp içinde

void MainWindow::setupRegisterTable(const DeviceConfig &config)
{
    qDebug() << "=== setupRegisterTable ===";
    qDebug() << "Config:";
    qDebug() << "  Quantity:" << config.quantity;
    qDebug() << "  Formats size:" << config.registerFormats.size();
    qDebug() << "  Start Address:" << config.startAddress;
    
    ui->registerTable->setRowCount(0);
    
    if (config.quantity <= 0) {
        qDebug() << "Invalid quantity:" << config.quantity;
        return;
    }
    
    if (config.registerFormats.isEmpty()) {
        qDebug() << "Register formats is empty";
        return;
    }
    
    for(int i = 0; i < config.quantity; i++) {
        qDebug() << "Adding row" << i;
        
        ui->registerTable->insertRow(i);
        
        // Adres formatını PLC tipine göre ayarla
        int rawAddress = config.startAddress + i;
        QString displayAddress;
        
        if (config.plcType.contains("Modicon")) {
            displayAddress = QString("4%1").arg(rawAddress, 4, 10, QChar('0'));
        }
        else if (config.plcType.contains("Delta")) {
            displayAddress = QString("2%1").arg(rawAddress - 2000, 3, 10, QChar('0'));
        }
        else if (config.plcType.contains("Siemens")) {
            displayAddress = QString("DB%1").arg(rawAddress);
        }
        else if (config.plcType.contains("Allen Bradley")) {
            displayAddress = QString("N7:%1").arg(rawAddress);
        }
        else {
            displayAddress = QString::number(rawAddress);
        }
        
        QTableWidgetItem* addressItem = new QTableWidgetItem(displayAddress);
        ui->registerTable->setItem(i, 0, addressItem);
        
        // Value
        QTableWidgetItem* valueItem = new QTableWidgetItem("0");
        ui->registerTable->setItem(i, 1, valueItem);
        
        // Format
        QString format = (i < config.registerFormats.size()) ? config.registerFormats[i] : "Dec";
        QTableWidgetItem* formatItem = new QTableWidgetItem(format);
        ui->registerTable->setItem(i, 2, formatItem);
        
        // Description
        QTableWidgetItem* descItem = new QTableWidgetItem("");
        ui->registerTable->setItem(i, 3, descItem);
    }
    qDebug() << "=== End setupRegisterTable ===";
}


void MainWindow::updateRegisters()
{
    for(auto it = deviceConfigs.begin(); it != deviceConfigs.end(); ++it) {
        int deviceId = it.key();
        auto &config = it.value();
        
        if(!config.isActive || !modbusContexts.contains(deviceId)) continue;
        
        modbus_t *ctx = modbusContexts[deviceId];
        uint16_t registers[128];
        
        // Adres dönüşümünü uygula
        int adjustedStartAddress = adjustRegisterAddress(config.startAddress, config.plcType);
        
        qDebug() << "Reading from device" << deviceId;
        qDebug() << "Original address:" << config.startAddress;
        qDebug() << "Adjusted address:" << adjustedStartAddress;
        qDebug() << "Quantity:" << config.quantity;
        
        // Okuma denemesi öncesi flush
        modbus_flush(ctx);
        
        int rc = modbus_read_registers(ctx, adjustedStartAddress, config.quantity, registers);
        
        if(rc == -1) {
            qDebug() << "Read error:" << modbus_strerror(errno);
            qDebug() << "Error code:" << errno;
            handleCommunicationError(deviceId, QString(modbus_strerror(errno)));
            continue;
        }
        
        qDebug() << "Successfully read" << rc << "registers";
        
        if(deviceId == currentDeviceId) {
            processReadResults(deviceId, registers, rc);
        }
    }
}

void MainWindow::onRegisterSetupClicked()
{
    if(!deviceConfigs.contains(currentDeviceId)) return;
    
    DeviceConfigDialog dialog(deviceConfigs[currentDeviceId], this);
	if(dialog.exec()) {
        setupRegisterTable(deviceConfigs[currentDeviceId]);
    }
}

void MainWindow::updateStatus(const QString &status)
{
    ui->statusbar->showMessage(status);
}

void MainWindow::saveSettings()
{
    QSettings settings("YourCompany", "QModbus");
    
    settings.beginGroup("Devices");
    settings.remove(""); // Mevcut ayarları temizle
    
    for(auto it = deviceConfigs.begin(); it != deviceConfigs.end(); ++it) {
        settings.setValue(QString::number(it.key()), deviceConfigToVariant(it.value()));
    }
    
    settings.endGroup();
    
    // Genel ayarları kaydet
    settings.setValue("PollRate", pollRate);
    settings.setValue("CurrentDeviceId", currentDeviceId);
}

void MainWindow::loadSettings()
{
    QSettings settings("YourCompany", "QModbus");
    
    // Cihaz yapılandırmalarını yükle
    settings.beginGroup("Devices");
    QStringList keys = settings.childKeys();
    
    for(const QString& key : keys) {
        int deviceId = key.toInt();
        deviceConfigs[deviceId] = variantToDeviceConfig(settings.value(key));
    }
    
    settings.endGroup();
    
    // Genel ayarları yükle
    pollRate = settings.value("PollRate", 1000).toInt();
    currentDeviceId = settings.value("CurrentDeviceId", 0).toInt();
    
    // UI'ı güncelle
    updateDeviceList();
    if(deviceConfigs.contains(currentDeviceId)) {
        setupRegisterTable(deviceConfigs[currentDeviceId]);
    }
}

double MainWindow::scaleValue(uint16_t rawValue, const RegisterConfig& config)
{
    return rawValue * config.scaleFactor;
}

void MainWindow::handleCommunicationError(int deviceId, const QString& error)
{
    qDebug() << "Communication error with device" << deviceId << ":" << error;
    updateStatus("Error: " + error);
    
    // Bağlantıyı kapat ve yeniden bağlanmayı dene
    if(modbusContexts.contains(deviceId)) {
        disconnectDevice(deviceId);
        QTimer::singleShot(5000, [this, deviceId]() {
            if(deviceConfigs.contains(deviceId)) {
                connectDevice(deviceId);
            }
        });
    }
}

bool MainWindow::validateRegisterConfig(const RegisterConfig& config)
{
    // Register adresinin geçerli aralıkta olup olmadığını kontrol et
    if(config.address < 0 || config.address > 65535) {
        return false;
    }
    
    // Veri tipinin desteklenip desteklenmediğini kontrol et
    QStringList supportedTypes = {"INT16", "UINT16", "FLOAT32", "BOOL"};
    if(!supportedTypes.contains(config.dataType)) {
        return false;
    }
    
    // Ölçekleme faktörünün geçerli olup olmadığını kontrol et
    if(config.scaleFactor == 0.0) {
        return false;
    }
    
    // Min/max değerlerinin mantıklı olup olmadığını kontrol et
    if(config.minValue >= config.maxValue) {
        return false;
    }
    
    return true;
}