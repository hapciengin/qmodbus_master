#include "DeviceConfigDialog.h"
#include "ui_DeviceConfigDialog.h"
#include <QDebug>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QDialogButtonBox>
#include <algorithm>

DeviceConfigDialog::DeviceConfigDialog(DeviceConfig &config, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DeviceConfigDialog)
    , deviceConfig(config)
    , isValid(false)    
{
    ui->setupUi(this);
    setupAddressRangeUI();
    setWindowTitle(tr("Device Configuration"));
    setModal(true);

    // Register tablosunu ayarla
    ui->registerTable->setColumnCount(5);
    ui->registerTable->setHorizontalHeaderLabels({
        tr("Address"), tr("Name"), tr("Data Type"), tr("Scale Factor"), tr("Unit")
    });
    ui->registerTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->registerTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->registerTable->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->registerTable->setAlternatingRowColors(true);

    // Sinyalleri bağla
    connect(ui->addButton, &QPushButton::clicked, this, &DeviceConfigDialog::onAddRegister);
    connect(ui->removeButton, &QPushButton::clicked, this, &DeviceConfigDialog::onRemoveRegister);
    connect(ui->okButton, &QPushButton::clicked, this, &DeviceConfigDialog::onOkClicked);
    connect(ui->cancelButton, &QPushButton::clicked, this, &DeviceConfigDialog::onCancelClicked);

    // Port için validator
    ui->portSpin->setRange(1, 65535);
    ui->portSpin->setValue(502);

    // Slave ID için validator
    ui->slaveIdSpin->setRange(1, 247);
    ui->slaveIdSpin->setValue(1);

    // IP adresi için validator
    QRegularExpression ipRegex("^(?:[0-9]{1,3}\\.){3}[0-9]{1,3}$");
    ui->ipEdit->setValidator(new QRegularExpressionValidator(ipRegex, this));

    // PLC tiplerini doldur
    ui->plcTypeCombo->addItems({
        "Siemens S7",
        "Allen Bradley",
        "Modicon",
        "Other"
    });

    // Register tiplerini doldur
    ui->registerTypeCombo->addItems({
        "Holding Register",
        "Input Register",
        "Coil",
        "Discrete Input"
    });

    // Data tiplerini doldur
    ui->dataTypeCombo->addItems({
        "INT16",
        "UINT16",
        "INT32",
        "UINT32",
        "FLOAT32",
        "BOOL"
    });

    // Byte order tiplerini doldur
    ui->byteOrderCombo->addItems({
        "AB CD (Big-Endian)",
        "CD AB (Little-Endian)",
        "BA DC (Big-Endian Byte Swap)",
        "DC BA (Little-Endian Byte Swap)"
    });

    // Timeout ayarları
    ui->timeoutSpin->setRange(100, 30000);
    ui->timeoutSpin->setSingleStep(100);
    ui->timeoutSpin->setValue(1000);
    ui->timeoutSpin->setSuffix(" ms");

    // Retry count ayarları
    ui->retryCountSpin->setRange(0, 10);
    ui->retryCountSpin->setValue(3);

    loadConfig();
    validateInput();
}

DeviceConfigDialog::~DeviceConfigDialog()
{
    delete ui;
}

void DeviceConfigDialog::onAddRegister()
{
    try {
        // Yeni satır ekle
        int row = ui->registerTable->rowCount();
        ui->registerTable->insertRow(row);

        // Yeni satır için varsayılan değerler
        QTableWidgetItem* items[] = {
            new QTableWidgetItem(QString::number(row)),  // Address
            new QTableWidgetItem(""),                    // Name
            new QTableWidgetItem("INT16"),               // Data Type
            new QTableWidgetItem("1.0"),                 // Scale Factor
            new QTableWidgetItem("")                     // Unit
        };

        // Her sütun için null kontrolü yaparak ekle
        for (int col = 0; col < 5; ++col) {
            if (items[col]) {
                items[col]->setFlags(items[col]->flags() | Qt::ItemIsEditable);
                ui->registerTable->setItem(row, col, items[col]);
            }
        }

        // Yeni satırı seç
        ui->registerTable->selectRow(row);
        ui->registerTable->scrollToItem(ui->registerTable->item(row, 0));
        
        // Değişiklikleri kaydet
        validateInput();
        emit configChanged();
    }
    catch (const std::exception& e) {
        qDebug() << "Exception in onAddRegister:" << e.what();
        QMessageBox::critical(this, tr("Error"), 
            tr("Failed to add register: %1").arg(e.what()));
    }
}

void DeviceConfigDialog::onRemoveRegister()
{
    QMutexLocker locker(&mutex);
    
    QList<QTableWidgetItem*> selectedItems = ui->registerTable->selectedItems();
    if (selectedItems.isEmpty()) {
        QMessageBox::warning(this, tr("Warning"), 
            tr("Please select a register to remove."));
        return;
    }

    QSet<int> rowsToRemove;
    for (QTableWidgetItem* item : selectedItems) {
        rowsToRemove.insert(item->row());
    }

    // Sıralı silme için rows'u tersten sırala
    QList<int> rows = rowsToRemove.values();
    std::sort(rows.begin(), rows.end(), std::greater<int>());

    for (int row : rows) {
        ui->registerTable->removeRow(row);
    }
}

void DeviceConfigDialog::onOkClicked()
{
    QMutexLocker locker(&mutex);
    
    try {
        if (!validateRegisterData()) {
            QMessageBox::warning(this, tr("Validation Error"),
                tr("Please check register configuration."));
            return;
        }

        // Temel ayarları kaydet
        deviceConfig.ip = ui->ipEdit->text();
        deviceConfig.port = ui->portSpin->value();
        deviceConfig.slaveId = ui->slaveIdSpin->value();
        deviceConfig.deviceName = ui->deviceNameEdit->text();
        deviceConfig.plcType = ui->plcTypeCombo->currentText();
        deviceConfig.timeout = ui->timeoutSpin->value();
        deviceConfig.retryCount = ui->retryCountSpin->value();

        // Register'ları kaydet
        deviceConfig.registers.clear();
        for (int row = 0; row < ui->registerTable->rowCount(); ++row) {
            RegisterConfig regConfig;
            
            bool ok;
            regConfig.address = ui->registerTable->item(row, 0)->text().toInt(&ok);
            if (!ok) continue;

            regConfig.name = ui->registerTable->item(row, 1)->text();
            regConfig.dataType = ui->registerTable->item(row, 2)->text();
            regConfig.scaleFactor = ui->registerTable->item(row, 3)->text().toDouble(&ok);
            if (!ok) regConfig.scaleFactor = 1.0;
            regConfig.unit = ui->registerTable->item(row, 4)->text();
            
            deviceConfig.registers[regConfig.address] = regConfig;
        }

        emit configChanged();
        accept();
    }
    catch (const std::exception& e) {
        QMessageBox::critical(this, tr("Error"), 
            tr("Failed to save configuration: %1").arg(e.what()));
    }
}

void DeviceConfigDialog::onCancelClicked()
{
    reject();
}

void DeviceConfigDialog::loadConfig()
{
    QMutexLocker locker(&mutex);
    
    // Temel ayarları yükle
    ui->ipEdit->setText(deviceConfig.ip);
    ui->portSpin->setValue(deviceConfig.port);
    ui->slaveIdSpin->setValue(deviceConfig.slaveId);
    ui->deviceNameEdit->setText(deviceConfig.deviceName);
    
    int plcIndex = ui->plcTypeCombo->findText(deviceConfig.plcType);
    if (plcIndex >= 0) ui->plcTypeCombo->setCurrentIndex(plcIndex);
    
    ui->timeoutSpin->setValue(deviceConfig.timeout);
    ui->retryCountSpin->setValue(deviceConfig.retryCount);

    // Register'ları yükle
    ui->registerTable->setRowCount(0);
    for (auto it = deviceConfig.registers.begin(); it != deviceConfig.registers.end(); ++it) {
        const RegisterConfig& reg = it.value();
        int row = ui->registerTable->rowCount();
        ui->registerTable->insertRow(row);

        ui->registerTable->setItem(row, 0, new QTableWidgetItem(QString::number(reg.address)));
        ui->registerTable->setItem(row, 1, new QTableWidgetItem(reg.name));
        ui->registerTable->setItem(row, 2, new QTableWidgetItem(reg.dataType));
        ui->registerTable->setItem(row, 3, new QTableWidgetItem(QString::number(reg.scaleFactor)));
        ui->registerTable->setItem(row, 4, new QTableWidgetItem(reg.unit));
    }
}

void DeviceConfigDialog::validateInput()
{
    QMutexLocker locker(&mutex);
    
    isValid = true;

    // IP adresi kontrolü
    QString ip = ui->ipEdit->text();
    QStringList parts = ip.split('.');
    if (parts.size() != 4) {
        isValid = false;
    } else {
        for (const QString& part : parts) {
            bool ok;
            int value = part.toInt(&ok);
            if (!ok || value < 0 || value > 255) {
                isValid = false;
                break;
            }
        }
    }

    // Port kontrolü
    int port = ui->portSpin->value();
    if (port <= 0 || port > 65535) {
        isValid = false;
    }

    updateUIState();
}

bool DeviceConfigDialog::validateRegisterData()
{
    for (int row = 0; row < ui->registerTable->rowCount(); ++row) {
        // Address kontrolü
        bool ok;
        int address = ui->registerTable->item(row, 0)->text().toInt(&ok);
        if (!ok || address < 0 || address > 65535) return false;

        // Scale factor kontrolü
        double scale = ui->registerTable->item(row, 3)->text().toDouble(&ok);
        if (!ok || scale == 0.0) return false;
    }
    return true;
}

void DeviceConfigDialog::updateUIState()
{
    QPalette palette = ui->ipEdit->palette();
    if (!isValid) {
        palette.setColor(QPalette::Text, Qt::red);
    } else {
        palette.setColor(QPalette::Text, palette.color(QPalette::WindowText));
    }
    ui->ipEdit->setPalette(palette);

    ui->okButton->setEnabled(isValid);
}

void DeviceConfigDialog::saveConfig()
{
    
}

void DeviceConfigDialog::setupAddressRangeUI()
{
    // Adres aralığı için yeni alanlar
    addressRangeLabel = new QLabel("Address Range:", this);
    startAddressBox = new QSpinBox(this);
    startAddressBox->setRange(0, 65535);
    startAddressBox->setValue(deviceConfig.startAddress);

    // Layout'a ekle
    QFormLayout* formLayout = qobject_cast<QFormLayout*>(ui->basicGroup->layout());
    if (formLayout) {
        formLayout->addRow(addressRangeLabel, startAddressBox);
    }

    // PLC tipine göre adres aralığını otomatik ayarla
    connect(ui->plcTypeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &DeviceConfigDialog::onPlcTypeChanged);
    connect(startAddressBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &DeviceConfigDialog::onAddressRangeChanged);

    // PLC tiplerini güncelle
    ui->plcTypeCombo->clear();
    ui->plcTypeCombo->addItems({
        "Modicon (40001-49999)",
        "Delta (2000-2999)",
        "Siemens (DB1-DB999)",
        "Allen Bradley (N7:0-N7:999)",
        "Custom Range"
    });
}

void DeviceConfigDialog::onPlcTypeChanged(int index)
{
    QString plcType = ui->plcTypeCombo->currentText();
    updateAddressRange(plcType);
}

void DeviceConfigDialog::updateAddressRange(const QString& plcType)
{
    if (plcType.contains("Modicon")) {
        startAddressBox->setRange(40001, 49999);
        startAddressBox->setValue(40001);
        deviceConfig.addressOffset = 40001;
        deviceConfig.addressFormat = "4xxxx";
    }
    else if (plcType.contains("Delta")) {
        startAddressBox->setRange(2000, 2999);
        startAddressBox->setValue(2000);
        deviceConfig.addressOffset = 2000;
        deviceConfig.addressFormat = "2xxx";
    }
    else if (plcType.contains("Siemens")) {
        startAddressBox->setRange(1, 999);
        startAddressBox->setValue(1);
        deviceConfig.addressOffset = 0;
        deviceConfig.addressFormat = "DBx";
    }
    else if (plcType.contains("Allen Bradley")) {
        startAddressBox->setRange(0, 999);
        startAddressBox->setValue(0);
        deviceConfig.addressOffset = 0;
        deviceConfig.addressFormat = "N7:x";
    }
    else {
        startAddressBox->setRange(0, 65535);
        startAddressBox->setValue(0);
        deviceConfig.addressOffset = 0;
        deviceConfig.addressFormat = "";
    }
}

void DeviceConfigDialog::onAddressRangeChanged(int value)
{
    deviceConfig.startAddress = value;
    validateInput();
}