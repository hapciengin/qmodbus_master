#include "ConnectionSettingsWidget.h"
#include "ui_ConnectionSettingsWidget.h"
#include <QSerialPortInfo>
#include <QNetworkInterface>
#include <QMessageBox>
#include <QSettings>
#include <QHostAddress>
#include <QRegularExpression>
#include <QRegularExpressionValidator>

ConnectionSettingsWidget::ConnectionSettingsWidget(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::ConnectionSettingsWidget)
    , isValidConfig(false)
{
    ui->setupUi(this);
    setupUi();
    setupConnections();
    setupValidators();
    populateComboBoxes();
    loadSettings();
    
    // İlk durumu ayarla
    updateUiState();
    validateSettings();
}

ConnectionSettingsWidget::~ConnectionSettingsWidget()
{
    saveSettings();
    delete ui;
}

void ConnectionSettingsWidget::setupUi()
{
    // Connection type mapping
    connectionTypeMap = {
        {ModbusTypes::ConnectionType::TCP_IP, 0},
        {ModbusTypes::ConnectionType::UDP_IP, 1},
        {ModbusTypes::ConnectionType::RTU_OVER_TCP, 2},
        {ModbusTypes::ConnectionType::RTU_OVER_UDP, 3},
        {ModbusTypes::ConnectionType::RTU_SERIAL, 4},
        {ModbusTypes::ConnectionType::ASCII_SERIAL, 5}
    };

    // Byte order mapping
    byteOrderMap = {
        {ModbusTypes::ByteOrder::AB_CD, 0},
        {ModbusTypes::ByteOrder::CD_AB, 1},
        {ModbusTypes::ByteOrder::BA_DC, 2},
        {ModbusTypes::ByteOrder::DC_BA, 3}
    };

    // Error recovery mode mapping
    errorRecoveryMap = {
        {ModbusTypes::ErrorRecoveryMode::NONE, 0},
        {ModbusTypes::ErrorRecoveryMode::LINK, 1},
        {ModbusTypes::ErrorRecoveryMode::PROTOCOL, 2},
        {ModbusTypes::ErrorRecoveryMode::LINK_AND_PROTOCOL, 3}
    };

    // Parity mapping
    parityMap = {
        {'N', 0},  // None
        {'E', 1},  // Even
        {'O', 2},  // Odd
        {'M', 3},  // Mark
        {'S', 4}   // Space
    };

    // Baud rate listesi
    QList<qint32> standardBaudRates = {
        1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200
    };
    ui->baudRateCombo->clear();
    for (qint32 rate : standardBaudRates) {
        ui->baudRateCombo->addItem(QString::number(rate), rate);
    }

    // Data bits listesi
    ui->dataBitsCombo->clear();
    ui->dataBitsCombo->addItem("5", 5);
    ui->dataBitsCombo->addItem("6", 6);
    ui->dataBitsCombo->addItem("7", 7);
    ui->dataBitsCombo->addItem("8", 8);

    // Stop bits listesi
    ui->stopBitsCombo->clear();
    ui->stopBitsCombo->addItem("1", 1);
    ui->stopBitsCombo->addItem("1.5", 3);
    ui->stopBitsCombo->addItem("2", 2);

    // Parity listesi
    ui->parityCombo->clear();
    ui->parityCombo->addItem("None", 'N');
    ui->parityCombo->addItem("Even", 'E');
    ui->parityCombo->addItem("Odd", 'O');
    ui->parityCombo->addItem("Mark", 'M');
    ui->parityCombo->addItem("Space", 'S');

    // Varsayılan değerler
    ui->portSpinBox->setValue(502);
    ui->slaveIdSpinBox->setValue(1);
    ui->timeoutSpinBox->setValue(1000);
    ui->retryCountSpinBox->setValue(3);
    ui->readWriteCheckBox->setChecked(true);
}

void ConnectionSettingsWidget::setupConnections()
{
    // Connection type değişimi
    connect(ui->connectionTypeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ConnectionSettingsWidget::onConnectionTypeChanged);

    // TCP/IP ayarları
    connect(ui->ipAddressEdit, &QLineEdit::textChanged,
            this, &ConnectionSettingsWidget::onIpAddressChanged);
    connect(ui->portSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &ConnectionSettingsWidget::onPortChanged);

    // Serial port ayarları
    connect(ui->serialPortCombo, &QComboBox::currentTextChanged,
            this, &ConnectionSettingsWidget::onSerialPortChanged);
    connect(ui->baudRateCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ConnectionSettingsWidget::onBaudRateChanged);
    connect(ui->parityCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ConnectionSettingsWidget::onParityChanged);
    connect(ui->dataBitsCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ConnectionSettingsWidget::onDataBitsChanged);
    connect(ui->stopBitsCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ConnectionSettingsWidget::onStopBitsChanged);

    // Genel ayarlar
    connect(ui->slaveIdSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &ConnectionSettingsWidget::onSlaveIdChanged);
    connect(ui->timeoutSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &ConnectionSettingsWidget::onTimeoutChanged);
    connect(ui->retryCountSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &ConnectionSettingsWidget::onRetryCountChanged);
    connect(ui->byteOrderCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ConnectionSettingsWidget::onByteOrderChanged);
    connect(ui->readWriteCheckBox, &QCheckBox::toggled,
            this, &ConnectionSettingsWidget::onReadWriteEnabledChanged);
    connect(ui->errorRecoveryCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ConnectionSettingsWidget::onErrorRecoveryModeChanged);

    // Refresh butonları
    connect(ui->refreshSerialButton, &QPushButton::clicked,
            this, &ConnectionSettingsWidget::updateSerialPorts);
    connect(ui->refreshNetworkButton, &QPushButton::clicked,
            this, &ConnectionSettingsWidget::updateNetworkInterfaces);
}

void ConnectionSettingsWidget::setupValidators()
{
    // IP adresi için validator
    QRegularExpression ipRegex("^(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}"
                              "(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$");
    ui->ipAddressEdit->setValidator(new QRegularExpressionValidator(ipRegex, this));

    // Port numarası için range
    ui->portSpinBox->setRange(1, 65535);

    // Slave ID için range
    ui->slaveIdSpinBox->setRange(1, 247);

    // Timeout için range (ms)
    ui->timeoutSpinBox->setRange(100, 30000);

    // Retry count için range
    ui->retryCountSpinBox->setRange(0, 10);
}

void ConnectionSettingsWidget::populateComboBoxes()
{
    // Connection types
    ui->connectionTypeCombo->clear();
    ui->connectionTypeCombo->addItem("TCP/IP", static_cast<int>(ModbusTypes::ConnectionType::TCP_IP));
    ui->connectionTypeCombo->addItem("UDP/IP", static_cast<int>(ModbusTypes::ConnectionType::UDP_IP));
    ui->connectionTypeCombo->addItem("RTU over TCP", static_cast<int>(ModbusTypes::ConnectionType::RTU_OVER_TCP));
    ui->connectionTypeCombo->addItem("RTU over UDP", static_cast<int>(ModbusTypes::ConnectionType::RTU_OVER_UDP));
    ui->connectionTypeCombo->addItem("RTU Serial", static_cast<int>(ModbusTypes::ConnectionType::RTU_SERIAL));
    ui->connectionTypeCombo->addItem("ASCII Serial", static_cast<int>(ModbusTypes::ConnectionType::ASCII_SERIAL));

    // Byte order
    ui->byteOrderCombo->clear();
    ui->byteOrderCombo->addItem("AB CD (Big-Endian)", static_cast<int>(ModbusTypes::ByteOrder::AB_CD));
    ui->byteOrderCombo->addItem("CD AB (Little-Endian)", static_cast<int>(ModbusTypes::ByteOrder::CD_AB));
    ui->byteOrderCombo->addItem("BA DC (Big-Endian Byte Swap)", static_cast<int>(ModbusTypes::ByteOrder::BA_DC));
    ui->byteOrderCombo->addItem("DC BA (Little-Endian Byte Swap)", static_cast<int>(ModbusTypes::ByteOrder::DC_BA));

    // Error recovery modes
    ui->errorRecoveryCombo->clear();
    ui->errorRecoveryCombo->addItem("None", static_cast<int>(ModbusTypes::ErrorRecoveryMode::NONE));
    ui->errorRecoveryCombo->addItem("Link", static_cast<int>(ModbusTypes::ErrorRecoveryMode::LINK));
    ui->errorRecoveryCombo->addItem("Protocol", static_cast<int>(ModbusTypes::ErrorRecoveryMode::PROTOCOL));
    ui->errorRecoveryCombo->addItem("Link & Protocol", static_cast<int>(ModbusTypes::ErrorRecoveryMode::LINK_AND_PROTOCOL));

    // Serial ports ve network interfaces'i güncelle
    updateSerialPorts();
    updateNetworkInterfaces();
}

void ConnectionSettingsWidget::updateSerialPorts()
{
    QString currentPort = ui->serialPortCombo->currentText();
    ui->serialPortCombo->clear();

    const auto ports = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo& info : ports) {
        QString portName = info.portName();
        QString description = info.description();
        if (!description.isEmpty()) {
            portName += QString(" (%1)").arg(description);
        }
        ui->serialPortCombo->addItem(portName, info.portName());
    }

    // Önceki seçili portu tekrar seç
    int index = ui->serialPortCombo->findData(currentPort);
    if (index >= 0) {
        ui->serialPortCombo->setCurrentIndex(index);
    }
}

void ConnectionSettingsWidget::updateNetworkInterfaces()
{
    QString currentIp = ui->ipAddressEdit->text();
    QComboBox* interfaceCombo = ui->networkInterfaceCombo;
    interfaceCombo->clear();

    interfaceCombo->addItem("Any", "0.0.0.0");
    interfaceCombo->addItem("Localhost", "127.0.0.1");

    const auto interfaces = QNetworkInterface::allInterfaces();
    for (const QNetworkInterface& iface : interfaces) {
        if (iface.flags().testFlag(QNetworkInterface::IsUp) &&
            iface.flags().testFlag(QNetworkInterface::IsRunning) &&
            !iface.flags().testFlag(QNetworkInterface::IsLoopBack)) {
            
            const auto addresses = iface.addressEntries();
            for (const QNetworkAddressEntry& addr : addresses) {
                QHostAddress ip = addr.ip();
                if (ip.protocol() == QAbstractSocket::IPv4Protocol) {
                    QString name = QString("%1 (%2)").arg(iface.name(), ip.toString());
                    interfaceCombo->addItem(name, ip.toString());
                }
            }
        }
    }

    // Önceki IP adresini seç
    int index = interfaceCombo->findData(currentIp);
    if (index >= 0) {
        interfaceCombo->setCurrentIndex(index);
    }
}

void ConnectionSettingsWidget::loadSettings()
{
    QSettings settings;
    settings.beginGroup("ConnectionSettings");

    // Connection type
    int connType = settings.value("ConnectionType", 0).toInt();
    ui->connectionTypeCombo->setCurrentIndex(connType);

    // TCP/IP settings
    ui->ipAddressEdit->setText(settings.value("IpAddress", "127.0.0.1").toString());
    ui->portSpinBox->setValue(settings.value("Port", 502).toInt());

    // Serial settings
    ui->serialPortCombo->setCurrentText(settings.value("SerialPort").toString());
    ui->baudRateCombo->setCurrentText(settings.value("BaudRate", "9600").toString());
    ui->parityCombo->setCurrentIndex(settings.value("Parity", 0).toInt());
    ui->dataBitsCombo->setCurrentIndex(settings.value("DataBits", 3).toInt());
    ui->stopBitsCombo->setCurrentIndex(settings.value("StopBits", 0).toInt());

    // Common settings
    ui->slaveIdSpinBox->setValue(settings.value("SlaveId", 1).toInt());
    ui->timeoutSpinBox->setValue(settings.value("Timeout", 1000).toInt());
    ui->retryCountSpinBox->setValue(settings.value("RetryCount", 3).toInt());
    ui->byteOrderCombo->setCurrentIndex(settings.value("ByteOrder", 0).toInt());
    ui->readWriteCheckBox->setChecked(settings.value("ReadWriteEnabled", true).toBool());
    ui->errorRecoveryCombo->setCurrentIndex(settings.value("ErrorRecovery", 0).toInt());

    settings.endGroup();
}

void ConnectionSettingsWidget::saveSettings()
{
    QSettings settings;
    settings.beginGroup("ConnectionSettings");

    // Connection type
    settings.setValue("ConnectionType", ui->connectionTypeCombo->currentIndex());

    // TCP/IP settings
    settings.setValue("IpAddress", ui->ipAddressEdit->text());
    settings.setValue("Port", ui->portSpinBox->value());

    // Serial settings
    settings.setValue("SerialPort", ui->serialPortCombo->currentData().toString());
    settings.setValue("BaudRate", ui->baudRateCombo->currentText());
    settings.setValue("Parity", ui->parityCombo->currentIndex());
    settings.setValue("DataBits", ui->dataBitsCombo->currentIndex());
    settings.setValue("StopBits", ui->stopBitsCombo->currentIndex());

    // Common settings
    settings.setValue("SlaveId", ui->slaveIdSpinBox->value());
    settings.setValue("Timeout", ui->timeoutSpinBox->value());
    settings.setValue("RetryCount", ui->retryCountSpinBox->value());
    settings.setValue("ByteOrder", ui->byteOrderCombo->currentIndex());
    settings.setValue("ReadWriteEnabled", ui->readWriteCheckBox->isChecked());
    settings.setValue("ErrorRecovery", ui->errorRecoveryCombo->currentIndex());

    settings.endGroup();
}

void ConnectionSettingsWidget::setConnectionParams(const ModbusTypes::ConnectionParams& params)
{
    currentParams = params;
    paramsToUi(params);
    validateSettings();
}

ModbusTypes::ConnectionParams ConnectionSettingsWidget::getConnectionParams() const
{
    return uiToParams();
}

bool ConnectionSettingsWidget::isValid() const
{
    return isValidConfig;
}

QString ConnectionSettingsWidget::getValidationError() const
{
    return validationError;
}

void ConnectionSettingsWidget::setEnabled(bool enabled)
{
    ui->mainWidget->setEnabled(enabled);
}

void ConnectionSettingsWidget::reset()
{
    // Default değerlere sıfırla
    ModbusTypes::ConnectionParams defaultParams;
    setConnectionParams(defaultParams);
}

void ConnectionSettingsWidget::showEvent(QShowEvent* event)
{
    QWidget::showEvent(event);
    updateSerialPorts();
    updateNetworkInterfaces();
}

void ConnectionSettingsWidget::changeEvent(QEvent* event)
{
    QWidget::changeEvent(event);
    if (event->type() == QEvent::LanguageChange) {
        ui->retranslateUi(this);
    }
}

void ConnectionSettingsWidget::onConnectionTypeChanged(int index)
{
    ModbusTypes::ConnectionType type = static_cast<ModbusTypes::ConnectionType>(
        ui->connectionTypeCombo->itemData(index).toInt());
    
    // TCP/Serial widget'ları göster/gizle
    bool isTcp = (type == ModbusTypes::ConnectionType::TCP_IP ||
                 type == ModbusTypes::ConnectionType::UDP_IP ||
                 type == ModbusTypes::ConnectionType::RTU_OVER_TCP ||
                 type == ModbusTypes::ConnectionType::RTU_OVER_UDP);
    
    ui->tcpSettingsWidget->setVisible(isTcp);
    ui->serialSettingsWidget->setVisible(!isTcp);

    validateSettings();
    emit connectionParamsChanged();
}

void ConnectionSettingsWidget::onIpAddressChanged(const QString& text)
{
    Q_UNUSED(text);
    validateSettings();
    emit connectionParamsChanged();
}

void ConnectionSettingsWidget::onPortChanged(int value)
{
    Q_UNUSED(value);
    validateSettings();
    emit connectionParamsChanged();
}

void ConnectionSettingsWidget::onSlaveIdChanged(int value)
{
    Q_UNUSED(value);
    validateSettings();
    emit connectionParamsChanged();
}

void ConnectionSettingsWidget::onTimeoutChanged(int value)
{
    Q_UNUSED(value);
    validateSettings();
    emit connectionParamsChanged();
}

void ConnectionSettingsWidget::onRetryCountChanged(int value)
{
    Q_UNUSED(value);
    validateSettings();
    emit connectionParamsChanged();
}

void ConnectionSettingsWidget::onByteOrderChanged(int index)
{
    Q_UNUSED(index);
    emit connectionParamsChanged();
}

void ConnectionSettingsWidget::onReadWriteEnabledChanged(bool enabled)
{
    Q_UNUSED(enabled);
    emit connectionParamsChanged();
}

void ConnectionSettingsWidget::onErrorRecoveryModeChanged(int index)
{
    Q_UNUSED(index);
    emit connectionParamsChanged();
}

void ConnectionSettingsWidget::onSerialPortChanged(const QString& port)
{
    Q_UNUSED(port);
    validateSettings();
    emit connectionParamsChanged();
}

void ConnectionSettingsWidget::onBaudRateChanged(int value)
{
    Q_UNUSED(value);
    validateSettings();
    emit connectionParamsChanged();
}

void ConnectionSettingsWidget::onParityChanged(int index)
{
    Q_UNUSED(index);
    validateSettings();
    emit connectionParamsChanged();
}

void ConnectionSettingsWidget::onDataBitsChanged(int value)
{
    Q_UNUSED(value);
    validateSettings();
    emit connectionParamsChanged();
}

void ConnectionSettingsWidget::onStopBitsChanged(int value)
{
    Q_UNUSED(value);
    validateSettings();
    emit connectionParamsChanged();
}

void ConnectionSettingsWidget::validateSettings()
{
    isValidConfig = true;
    validationError.clear();

    // Connection type kontrolü
    ModbusTypes::ConnectionType type = static_cast<ModbusTypes::ConnectionType>(
        ui->connectionTypeCombo->currentData().toInt());

    // TCP/IP ayarları kontrolü
    if (type == ModbusTypes::ConnectionType::TCP_IP ||
        type == ModbusTypes::ConnectionType::UDP_IP ||
        type == ModbusTypes::ConnectionType::RTU_OVER_TCP ||
        type == ModbusTypes::ConnectionType::RTU_OVER_UDP) {
        
        if (!validateIpAddress(ui->ipAddressEdit->text())) {
            isValidConfig = false;
            validationError = tr("Invalid IP address");
        }
    }
    // Serial port ayarları kontrolü
    else {
        if (!validateSerialPort(ui->serialPortCombo->currentData().toString())) {
            isValidConfig = false;
            validationError = tr("Invalid serial port");
        }
    }

    // Slave ID kontrolü
    int slaveId = ui->slaveIdSpinBox->value();
    if (slaveId < 1 || slaveId > 247) {
        isValidConfig = false;
        validationError = tr("Slave ID must be between 1 and 247");
    }

    // Timeout kontrolü
    int timeout = ui->timeoutSpinBox->value();
    if (timeout < 100 || timeout > 30000) {
        isValidConfig = false;
        validationError = tr("Timeout must be between 100ms and 30000ms");
    }

    // UI durumunu güncelle
    updateUiState();
    emit validationStateChanged(isValidConfig);
}

bool ConnectionSettingsWidget::validateIpAddress(const QString& address) const
{
    QHostAddress addr;
    if (!addr.setAddress(address)) {
        return false;
    }

    // Sadece IPv4 adreslerini kabul et
    return (addr.protocol() == QAbstractSocket::IPv4Protocol);
}

bool ConnectionSettingsWidget::validateSerialPort(const QString& port) const
{
    if (port.isEmpty()) {
        return false;
    }

    // Port mevcut mu kontrol et
    const auto ports = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo& info : ports) {
        if (info.portName() == port) {
            return true;
        }
    }

    return false;
}

void ConnectionSettingsWidget::updateUiState()
{
    // Validation durumuna göre UI'ı güncelle
    QPalette palette = ui->ipAddressEdit->palette();
    if (!isValidConfig) {
        palette.setColor(QPalette::Text, Qt::red);
    } else {
        palette.setColor(QPalette::Text, palette.color(QPalette::WindowText));
    }
    ui->ipAddressEdit->setPalette(palette);

    // Hata mesajını göster/gizle
    ui->errorLabel->setVisible(!isValidConfig);
    ui->errorLabel->setText(validationError);
}

ModbusTypes::ConnectionParams ConnectionSettingsWidget::uiToParams() const
{
    ModbusTypes::ConnectionParams params;

    // Connection type
    params.type = static_cast<ModbusTypes::ConnectionType>(
        ui->connectionTypeCombo->currentData().toInt());

    // TCP/IP settings
    params.ip = ui->ipAddressEdit->text();
    params.port = ui->portSpinBox->value();

    // Serial settings
    params.serialPort = ui->serialPortCombo->currentData().toString();
    params.baudRate = ui->baudRateCombo->currentText().toInt();
    params.parity = ui->parityCombo->currentData().toChar().toLatin1();
    params.dataBits = ui->dataBitsCombo->currentData().toInt();
    params.stopBits = ui->stopBitsCombo->currentData().toInt();

    // Common settings
    params.slaveId = ui->slaveIdSpinBox->value();
    params.timeout = ui->timeoutSpinBox->value();
    params.retryCount = ui->retryCountSpinBox->value();
    params.byteOrder = static_cast<ModbusTypes::ByteOrder>(
        ui->byteOrderCombo->currentData().toInt());
    params.readWriteEnabled = ui->readWriteCheckBox->isChecked();
    params.errorRecovery = static_cast<ModbusTypes::ErrorRecoveryMode>(
        ui->errorRecoveryCombo->currentData().toInt());

    return params;
}

void ConnectionSettingsWidget::paramsToUi(const ModbusTypes::ConnectionParams& params)
{
    // Connection type
    int typeIndex = ui->connectionTypeCombo->findData(
        static_cast<int>(params.type));
    if (typeIndex >= 0) {
        ui->connectionTypeCombo->setCurrentIndex(typeIndex);
    }

    // TCP/IP settings
    ui->ipAddressEdit->setText(params.ip);
    ui->portSpinBox->setValue(params.port);

    // Serial settings
    int portIndex = ui->serialPortCombo->findData(params.serialPort);
    if (portIndex >= 0) {
        ui->serialPortCombo->setCurrentIndex(portIndex);
    }
    int baudIndex = ui->baudRateCombo->findText(QString::number(params.baudRate));
    if (baudIndex >= 0) {
        ui->baudRateCombo->setCurrentIndex(baudIndex);
    }
    int parityIndex = ui->parityCombo->findData(params.parity);
    if (parityIndex >= 0) {
        ui->parityCombo->setCurrentIndex(parityIndex);
    }
    int dataBitsIndex = ui->dataBitsCombo->findData(params.dataBits);
    if (dataBitsIndex >= 0) {
        ui->dataBitsCombo->setCurrentIndex(dataBitsIndex);
    }
    int stopBitsIndex = ui->stopBitsCombo->findData(params.stopBits);
    if (stopBitsIndex >= 0) {
        ui->stopBitsCombo->setCurrentIndex(stopBitsIndex);
    }

    // Common settings
    ui->slaveIdSpinBox->setValue(params.slaveId);
    ui->timeoutSpinBox->setValue(params.timeout);
    ui->retryCountSpinBox->setValue(params.retryCount);
    int byteOrderIndex = ui->byteOrderCombo->findData(
        static_cast<int>(params.byteOrder));
    if (byteOrderIndex >= 0) {
        ui->byteOrderCombo->setCurrentIndex(byteOrderIndex);
    }
    ui->readWriteCheckBox->setChecked(params.readWriteEnabled);
    int recoveryIndex = ui->errorRecoveryCombo->findData(
        static_cast<int>(params.errorRecovery));
    if (recoveryIndex >= 0) {
        ui->errorRecoveryCombo->setCurrentIndex(recoveryIndex);
    }

    // UI durumunu güncelle
    updateConnectionTypeWidgets();
    validateSettings();
}

void ConnectionSettingsWidget::updateConnectionTypeWidgets()
{
    ModbusTypes::ConnectionType type = static_cast<ModbusTypes::ConnectionType>(
        ui->connectionTypeCombo->currentData().toInt());
    
    bool isTcp = (type == ModbusTypes::ConnectionType::TCP_IP ||
                 type == ModbusTypes::ConnectionType::UDP_IP ||
                 type == ModbusTypes::ConnectionType::RTU_OVER_TCP ||
                 type == ModbusTypes::ConnectionType::RTU_OVER_UDP);
    
    ui->tcpSettingsWidget->setVisible(isTcp);
    ui->serialSettingsWidget->setVisible(!isTcp);
}