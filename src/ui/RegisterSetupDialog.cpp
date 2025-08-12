// RegisterSetupDialog.cpp - Part 1
#include "RegisterSetupDialog.h"
#include "ui_RegisterSetupDialog.h"
#include <QMessageBox>
#include <QRegularExpressionValidator>
#include <QShowEvent>
#include <QDebug>

RegisterSetupDialog::RegisterSetupDialog(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::RegisterSetupDialog)
    , multiEditMode(false)
    , isValidConfig(false)
{
    ui->setupUi(this);
    setupUi();
    setupConnections();
    setupValidators();
    setupDataTypeLimits();
    populateComboBoxes();
    
    // Varsayılan yapılandırma
    setRegisterConfig(ModbusTypes::RegisterConfig());
}

RegisterSetupDialog::~RegisterSetupDialog()
{
    delete ui;
}

void RegisterSetupDialog::setupUi()
{
    // Data type mapping
    dataTypeMap = {
        {ModbusTypes::DataType::BIT, 0},
        {ModbusTypes::DataType::BYTE, 1},
        {ModbusTypes::DataType::WORD, 2},
        {ModbusTypes::DataType::INT, 3},
        {ModbusTypes::DataType::DWORD, 4},
        {ModbusTypes::DataType::DINT, 5},
        {ModbusTypes::DataType::REAL, 6},
        {ModbusTypes::DataType::LREAL, 7},
        {ModbusTypes::DataType::STRING, 8},
        {ModbusTypes::DataType::WSTRING, 9}
    };

    // Register type mapping
    registerTypeMap = {
        {ModbusTypes::RegisterType::COIL, 0},
        {ModbusTypes::RegisterType::DISCRETE_INPUT, 1},
        {ModbusTypes::RegisterType::INPUT_REGISTER, 2},
        {ModbusTypes::RegisterType::HOLDING_REGISTER, 3}
    };

    // Byte order mapping
    byteOrderMap = {
        {ModbusTypes::ByteOrder::AB_CD, 0},
        {ModbusTypes::ByteOrder::CD_AB, 1},
        {ModbusTypes::ByteOrder::BA_DC, 2},
        {ModbusTypes::ByteOrder::DC_BA, 3}
    };

    // Hata etiketi başlangıçta gizli
    ui->errorLabel->setVisible(false);
    ui->errorLabel->setStyleSheet("QLabel { color: red; }");

    // Preview widget'ı başlangıçta gizli
    ui->previewGroupBox->setVisible(false);
}

void RegisterSetupDialog::setupConnections()
{
    // Temel özellikler
    connect(ui->addressSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &RegisterSetupDialog::onAddressChanged);
    connect(ui->nameEdit, &QLineEdit::textChanged,
            this, &RegisterSetupDialog::onNameChanged);
    connect(ui->descriptionEdit, &QLineEdit::textChanged,
            this, &RegisterSetupDialog::onDescriptionChanged);

    // Veri tipi ve erişim
    connect(ui->dataTypeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &RegisterSetupDialog::onDataTypeChanged);
    connect(ui->registerTypeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &RegisterSetupDialog::onRegisterTypeChanged);
    connect(ui->accessTypeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &RegisterSetupDialog::onAccessTypeChanged);
    connect(ui->byteOrderCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &RegisterSetupDialog::onByteOrderChanged);

    // Ölçekleme ve birim
    connect(ui->scaleFactorSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &RegisterSetupDialog::onScaleFactorChanged);
    connect(ui->unitEdit, &QLineEdit::textChanged,
            this, &RegisterSetupDialog::onUnitChanged);

    // Min/Max değerler
    connect(ui->minValueSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &RegisterSetupDialog::onMinValueChanged);
    connect(ui->maxValueSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &RegisterSetupDialog::onMaxValueChanged);

    // Alarm ayarları
    connect(ui->alarmEnabledCheck, &QCheckBox::toggled,
            this, &RegisterSetupDialog::onAlarmEnabledChanged);
    connect(ui->alarmLowLimitSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &RegisterSetupDialog::onAlarmLowLimitChanged);
    connect(ui->alarmHighLimitSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &RegisterSetupDialog::onAlarmHighLimitChanged);

    // Preview güncelleme
    connect(this, &RegisterSetupDialog::configChanged,
            this, &RegisterSetupDialog::updatePreview);
}

void RegisterSetupDialog::setupValidators()
{
    // İsim için validator (alfanumerik ve alt çizgi)
    QRegularExpression nameRegex("^[a-zA-Z][a-zA-Z0-9_]*$");
    ui->nameEdit->setValidator(new QRegularExpressionValidator(nameRegex, this));

    // Birim için validator (alfanumerik ve bazı özel karakterler)
    QRegularExpression unitRegex("^[a-zA-Z0-9°%/\\-\\s]*$");
    ui->unitEdit->setValidator(new QRegularExpressionValidator(unitRegex, this));

    // SpinBox'lar için varsayılan aralıklar
    ui->addressSpinBox->setRange(0, 65535);
    ui->scaleFactorSpinBox->setRange(-1e9, 1e9);
    ui->scaleFactorSpinBox->setDecimals(6);
}
// RegisterSetupDialog.cpp - Part 2

void RegisterSetupDialog::setupDataTypeLimits()
{
    // Her veri tipi için sınırları tanımla
    dataTypeLimits = {
        {ModbusTypes::DataType::BIT, {0, 1, 0}},
        {ModbusTypes::DataType::BYTE, {0, 255, 0}},
        {ModbusTypes::DataType::WORD, {0, 65535, 0}},
        {ModbusTypes::DataType::INT, {-32768, 32767, 0}},
        {ModbusTypes::DataType::DWORD, {0, 4294967295, 0}},
        {ModbusTypes::DataType::DINT, {-2147483648, 2147483647, 0}},
        {ModbusTypes::DataType::REAL, {-3.4e38, 3.4e38, 6}},
        {ModbusTypes::DataType::LREAL, {-1.7e308, 1.7e308, 15}},
        {ModbusTypes::DataType::STRING, {0, 0, 0}},  // String için sınır yok
        {ModbusTypes::DataType::WSTRING, {0, 0, 0}}  // WString için sınır yok
    };
}

void RegisterSetupDialog::populateComboBoxes()
{
    // Data type combo
    ui->dataTypeCombo->clear();
    ui->dataTypeCombo->addItem("BIT", static_cast<int>(ModbusTypes::DataType::BIT));
    ui->dataTypeCombo->addItem("BYTE", static_cast<int>(ModbusTypes::DataType::BYTE));
    ui->dataTypeCombo->addItem("WORD", static_cast<int>(ModbusTypes::DataType::WORD));
    ui->dataTypeCombo->addItem("INT", static_cast<int>(ModbusTypes::DataType::INT));
    ui->dataTypeCombo->addItem("DWORD", static_cast<int>(ModbusTypes::DataType::DWORD));
    ui->dataTypeCombo->addItem("DINT", static_cast<int>(ModbusTypes::DataType::DINT));
    ui->dataTypeCombo->addItem("REAL", static_cast<int>(ModbusTypes::DataType::REAL));
    ui->dataTypeCombo->addItem("LREAL", static_cast<int>(ModbusTypes::DataType::LREAL));
    ui->dataTypeCombo->addItem("STRING", static_cast<int>(ModbusTypes::DataType::STRING));
    ui->dataTypeCombo->addItem("WSTRING", static_cast<int>(ModbusTypes::DataType::WSTRING));

    // Register type combo
    ui->registerTypeCombo->clear();
    ui->registerTypeCombo->addItem("Coil", static_cast<int>(ModbusTypes::RegisterType::COIL));
    ui->registerTypeCombo->addItem("Discrete Input", static_cast<int>(ModbusTypes::RegisterType::DISCRETE_INPUT));
    ui->registerTypeCombo->addItem("Input Register", static_cast<int>(ModbusTypes::RegisterType::INPUT_REGISTER));
    ui->registerTypeCombo->addItem("Holding Register", static_cast<int>(ModbusTypes::RegisterType::HOLDING_REGISTER));

    // Access type combo
    ui->accessTypeCombo->clear();
    ui->accessTypeCombo->addItem("Read/Write");
    ui->accessTypeCombo->addItem("Read Only");

    // Byte order combo
    ui->byteOrderCombo->clear();
    ui->byteOrderCombo->addItem("AB CD (Big-Endian)", static_cast<int>(ModbusTypes::ByteOrder::AB_CD));
    ui->byteOrderCombo->addItem("CD AB (Little-Endian)", static_cast<int>(ModbusTypes::ByteOrder::CD_AB));
    ui->byteOrderCombo->addItem("BA DC (Big-Endian Byte Swap)", static_cast<int>(ModbusTypes::ByteOrder::BA_DC));
    ui->byteOrderCombo->addItem("DC BA (Little-Endian Byte Swap)", static_cast<int>(ModbusTypes::ByteOrder::DC_BA));
}

void RegisterSetupDialog::setRegisterConfig(const ModbusTypes::RegisterConfig& config)
{
    currentConfig = config;
    configToUi(config);
    validateInput();
    updatePreview();
}

ModbusTypes::RegisterConfig RegisterSetupDialog::getRegisterConfig() const
{
    return uiToConfig();
}

void RegisterSetupDialog::setMultiEditMode(bool enabled)
{
    if (multiEditMode != enabled) {
        multiEditMode = enabled;
        ui->previewGroupBox->setVisible(!enabled);
        updateUIState();
    }
}

bool RegisterSetupDialog::isMultiEditMode() const
{
    return multiEditMode;
}

void RegisterSetupDialog::setEditableFields(const QSet<QString>& fields)
{
    editableFields = fields;
    updateEditableState();
}

bool RegisterSetupDialog::isValid() const
{
    return isValidConfig;
}

QString RegisterSetupDialog::getValidationError() const
{
    return validationError;
}

void RegisterSetupDialog::accept()
{
    if (!validateInput()) {
        QMessageBox::warning(this, tr("Validation Error"),
                           tr("Please correct the following error:\n%1")
                           .arg(validationError));
        return;
    }
    QDialog::accept();
}

void RegisterSetupDialog::showEvent(QShowEvent* event)
{
    QDialog::showEvent(event);
    validateInput();
    updatePreview();
}

void RegisterSetupDialog::changeEvent(QEvent* event)
{
    QDialog::changeEvent(event);
    if (event->type() == QEvent::LanguageChange) {
        ui->retranslateUi(this);
    }
}
// RegisterSetupDialog.cpp - Part 3

void RegisterSetupDialog::onAddressChanged(int value)
{
    currentConfig.address = value;
    validateInput();
    emit configChanged();
}

void RegisterSetupDialog::onNameChanged(const QString& text)
{
    currentConfig.name = text;
    validateInput();
    emit configChanged();
}

void RegisterSetupDialog::onDescriptionChanged(const QString& text)
{
    currentConfig.description = text;
    emit configChanged();
}

void RegisterSetupDialog::onDataTypeChanged(int index)
{
    currentConfig.dataType = static_cast<ModbusTypes::DataType>(
        ui->dataTypeCombo->itemData(index).toInt());
    
    // Veri tipine göre UI'ı güncelle
    updateDataTypeUI();
    updateMinMaxRanges();
    validateInput();
    emit configChanged();
}

void RegisterSetupDialog::onRegisterTypeChanged(int index)
{
    currentConfig.regType = static_cast<ModbusTypes::RegisterType>(
        ui->registerTypeCombo->itemData(index).toInt());
    
    // Register tipine göre erişim tipini güncelle
    bool isReadOnly = (currentConfig.regType == ModbusTypes::RegisterType::DISCRETE_INPUT ||
                      currentConfig.regType == ModbusTypes::RegisterType::INPUT_REGISTER);
    ui->accessTypeCombo->setCurrentIndex(isReadOnly ? 1 : 0);
    ui->accessTypeCombo->setEnabled(!isReadOnly);
    
    validateInput();
    emit configChanged();
}

void RegisterSetupDialog::onAccessTypeChanged(int index)
{
    currentConfig.isReadOnly = (index == 1);
    emit configChanged();
}

void RegisterSetupDialog::onByteOrderChanged(int index)
{
    currentConfig.byteOrder = static_cast<ModbusTypes::ByteOrder>(
        ui->byteOrderCombo->itemData(index).toInt());
    emit configChanged();
}

void RegisterSetupDialog::onScaleFactorChanged(double value)
{
    currentConfig.scaleFactor = value;
    validateInput();
    emit configChanged();
}

void RegisterSetupDialog::onUnitChanged(const QString& text)
{
    currentConfig.unit = text;
    emit configChanged();
}

void RegisterSetupDialog::onMinValueChanged(double value)
{
    currentConfig.minValue = value;
    
    // Max değerin minimum sınırını güncelle
    ui->maxValueSpinBox->setMinimum(value);
    
    // Alarm alt limitinin minimum sınırını güncelle
    ui->alarmLowLimitSpinBox->setMinimum(value);
    
    validateInput();
    emit configChanged();
}

void RegisterSetupDialog::onMaxValueChanged(double value)
{
    currentConfig.maxValue = value;
    
    // Min değerin maximum sınırını güncelle
    ui->minValueSpinBox->setMaximum(value);
    
    // Alarm üst limitinin maximum sınırını güncelle
    ui->alarmHighLimitSpinBox->setMaximum(value);
    
    validateInput();
    emit configChanged();
}

void RegisterSetupDialog::onAlarmEnabledChanged(bool enabled)
{
    currentConfig.isAlarmEnabled = enabled;
    updateAlarmUI();
    validateInput();
    emit configChanged();
}

void RegisterSetupDialog::onAlarmLowLimitChanged(double value)
{
    currentConfig.alarmLowLimit = value;
    
    // Alarm üst limitinin minimum sınırını güncelle
    ui->alarmHighLimitSpinBox->setMinimum(value);
    
    validateInput();
    emit configChanged();
}

void RegisterSetupDialog::onAlarmHighLimitChanged(double value)
{
    currentConfig.alarmHighLimit = value;
    
    // Alarm alt limitinin maximum sınırını güncelle
    ui->alarmLowLimitSpinBox->setMaximum(value);
    
    validateInput();
    emit configChanged();
}

void RegisterSetupDialog::updateDataTypeUI()
{
    // Byte order seçeneğini sadece çoklu byte veri tipleri için göster
    bool showByteOrder = (currentConfig.dataType == ModbusTypes::DataType::WORD ||
                         currentConfig.dataType == ModbusTypes::DataType::INT ||
                         currentConfig.dataType == ModbusTypes::DataType::DWORD ||
                         currentConfig.dataType == ModbusTypes::DataType::DINT ||
                         currentConfig.dataType == ModbusTypes::DataType::REAL ||
                         currentConfig.dataType == ModbusTypes::DataType::LREAL);
    
    ui->byteOrderLabel->setVisible(showByteOrder);
    ui->byteOrderCombo->setVisible(showByteOrder);
    
    // Ölçekleme seçeneğini sadece sayısal veri tipleri için göster
    bool showScaling = (currentConfig.dataType != ModbusTypes::DataType::BIT &&
                       currentConfig.dataType != ModbusTypes::DataType::STRING &&
                       currentConfig.dataType != ModbusTypes::DataType::WSTRING);
    
    ui->scaleFactorLabel->setVisible(showScaling);
    ui->scaleFactorSpinBox->setVisible(showScaling);
    
    // SpinBox'ların hassasiyetini güncelle
    updateSpinBoxPrecision();
}

void RegisterSetupDialog::updateAlarmUI()
{
    bool alarmsEnabled = ui->alarmEnabledCheck->isChecked();
    
    ui->alarmLowLimitLabel->setEnabled(alarmsEnabled);
    ui->alarmLowLimitSpinBox->setEnabled(alarmsEnabled);
    ui->alarmHighLimitLabel->setEnabled(alarmsEnabled);
    ui->alarmHighLimitSpinBox->setEnabled(alarmsEnabled);
}
// RegisterSetupDialog.cpp - Part 4 (Final)

bool RegisterSetupDialog::validateInput()
{
    isValidConfig = true;
    validationError.clear();

    // Adres validasyonu
    if (!validateAddress()) {
        isValidConfig = false;
        validationError = tr("Invalid address");
    }
    // İsim validasyonu
    else if (!validateName()) {
        isValidConfig = false;
        validationError = tr("Invalid name");
    }
    // Ölçekleme validasyonu
    else if (!validateScale()) {
        isValidConfig = false;
        validationError = tr("Invalid scale factor");
    }
    // Min/Max değer validasyonu
    else if (!validateMinMax()) {
        isValidConfig = false;
        validationError = tr("Invalid min/max values");
    }
    // Alarm limitleri validasyonu
    else if (!validateAlarmLimits()) {
        isValidConfig = false;
        validationError = tr("Invalid alarm limits");
    }

    // UI'ı güncelle
    updateUIState();
    emit validationStateChanged(isValidConfig);
}

bool RegisterSetupDialog::validateAddress() const
{
    int address = ui->addressSpinBox->value();
    return (address >= 0 && address <= 65535);
}

bool RegisterSetupDialog::validateName() const
{
    QString name = ui->nameEdit->text();
    if (name.isEmpty()) {
        return false;
    }
    
    // İsim alfanumerik ve alt çizgi içerebilir, rakamla başlayamaz
    QRegularExpression nameRegex("^[a-zA-Z][a-zA-Z0-9_]*$");
    return nameRegex.match(name).hasMatch();
}

bool RegisterSetupDialog::validateScale() const
{
    if (currentConfig.dataType == ModbusTypes::DataType::BIT ||
        currentConfig.dataType == ModbusTypes::DataType::STRING ||
        currentConfig.dataType == ModbusTypes::DataType::WSTRING) {
        return true;  // Bu veri tipleri için ölçekleme kullanılmaz
    }
    
    double scale = ui->scaleFactorSpinBox->value();
    return scale != 0.0;  // Sıfıra bölme hatası olmaması için
}

bool RegisterSetupDialog::validateMinMax() const
{
    if (currentConfig.dataType == ModbusTypes::DataType::BIT ||
        currentConfig.dataType == ModbusTypes::DataType::STRING ||
        currentConfig.dataType == ModbusTypes::DataType::WSTRING) {
        return true;  // Bu veri tipleri için min/max kullanılmaz
    }
    
    double min = ui->minValueSpinBox->value();
    double max = ui->maxValueSpinBox->value();
    return min < max;
}

bool RegisterSetupDialog::validateAlarmLimits() const
{
    if (!ui->alarmEnabledCheck->isChecked()) {
        return true;  // Alarmlar devre dışıysa validasyon gerekmez
    }
    
    double low = ui->alarmLowLimitSpinBox->value();
    double high = ui->alarmHighLimitSpinBox->value();
    double min = ui->minValueSpinBox->value();
    double max = ui->maxValueSpinBox->value();
    
    return (low >= min && low < high && high <= max);
}

void RegisterSetupDialog::updatePreview()
{
    if (multiEditMode) {
        return;  // Çoklu düzenleme modunda preview gösterme
    }

    // Preview metnini oluştur
    QString preview;
    QTextStream stream(&preview);
    
    stream << "Address: " << formatAddress(currentConfig.address) << "\n";
    stream << "Type: " << getDataTypeString(currentConfig.dataType) << "\n";
    stream << "Access: " << (currentConfig.isReadOnly ? "Read Only" : "Read/Write") << "\n";
    
    if (currentConfig.dataType != ModbusTypes::DataType::BIT &&
        currentConfig.dataType != ModbusTypes::DataType::STRING &&
        currentConfig.dataType != ModbusTypes::DataType::WSTRING) {
        
        stream << "Scale: " << QString::number(currentConfig.scaleFactor, 'g', 6) << "\n";
        stream << "Range: " << QString::number(currentConfig.minValue, 'g', 6)
               << " to " << QString::number(currentConfig.maxValue, 'g', 6);
        
        if (!currentConfig.unit.isEmpty()) {
            stream << " " << currentConfig.unit;
        }
        stream << "\n";
        
        if (currentConfig.isAlarmEnabled) {
            stream << "Alarm Limits: " << QString::number(currentConfig.alarmLowLimit, 'g', 6)
                   << " to " << QString::number(currentConfig.alarmHighLimit, 'g', 6);
            if (!currentConfig.unit.isEmpty()) {
                stream << " " << currentConfig.unit;
            }
            stream << "\n";
        }
    }

    ui->previewText->setPlainText(preview);
}

void RegisterSetupDialog::updateUIState()
{
    // Validasyon durumunu göster
    ui->errorLabel->setVisible(!isValidConfig);
    ui->errorLabel->setText(validationError);
    
    // OK butonunu aktif/pasif yap
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(isValidConfig);
    
    // Düzenlenebilir alanları güncelle
    updateEditableState();
}

void RegisterSetupDialog::updateEditableState()
{
    // Çoklu düzenleme modunda sadece seçili alanları düzenlenebilir yap
    if (multiEditMode) {
        ui->addressSpinBox->setEnabled(false);  // Adres asla düzenlenemez
        ui->nameEdit->setEnabled(editableFields.contains("name"));
        ui->descriptionEdit->setEnabled(editableFields.contains("description"));
        ui->dataTypeCombo->setEnabled(editableFields.contains("dataType"));
        ui->registerTypeCombo->setEnabled(editableFields.contains("regType"));
        ui->accessTypeCombo->setEnabled(editableFields.contains("access"));
        ui->byteOrderCombo->setEnabled(editableFields.contains("byteOrder"));
        ui->scaleFactorSpinBox->setEnabled(editableFields.contains("scale"));
        ui->unitEdit->setEnabled(editableFields.contains("unit"));
        ui->minValueSpinBox->setEnabled(editableFields.contains("minValue"));
        ui->maxValueSpinBox->setEnabled(editableFields.contains("maxValue"));
        ui->alarmEnabledCheck->setEnabled(editableFields.contains("alarmEnabled"));
        ui->alarmLowLimitSpinBox->setEnabled(editableFields.contains("alarmLowLimit"));
        ui->alarmHighLimitSpinBox->setEnabled(editableFields.contains("alarmHighLimit"));
    }
}

QString RegisterSetupDialog::formatAddress(int address) const
{
    return QString("0x%1 (%2)")
            .arg(address, 4, 16, QChar('0'))
            .arg(address);
}

QString RegisterSetupDialog::getDataTypeString(ModbusTypes::DataType type) const
{
    switch (type) {
        case ModbusTypes::DataType::BIT: return "BIT";
        case ModbusTypes::DataType::BYTE: return "BYTE";
        case ModbusTypes::DataType::WORD: return "WORD";
        case ModbusTypes::DataType::INT: return "INT";
        case ModbusTypes::DataType::DWORD: return "DWORD";
        case ModbusTypes::DataType::DINT: return "DINT";
        case ModbusTypes::DataType::REAL: return "REAL";
        case ModbusTypes::DataType::LREAL: return "LREAL";
        case ModbusTypes::DataType::STRING: return "STRING";
        case ModbusTypes::DataType::WSTRING: return "WSTRING";
        default: return "UNKNOWN";
    }
}

void RegisterSetupDialog::updateSpinBoxPrecision()
{
    auto it = dataTypeLimits.find(currentConfig.dataType);
    if (it != dataTypeLimits.end()) {
        const auto& limits = it.value();
        
        ui->minValueSpinBox->setDecimals(limits.precision);
        ui->maxValueSpinBox->setDecimals(limits.precision);
        ui->alarmLowLimitSpinBox->setDecimals(limits.precision);
        ui->alarmHighLimitSpinBox->setDecimals(limits.precision);
        
        ui->minValueSpinBox->setRange(limits.minValue, limits.maxValue);
        ui->maxValueSpinBox->setRange(limits.minValue, limits.maxValue);
    }
}
void RegisterSetupDialog::configToUi(const ModbusTypes::RegisterConfig& config) {
    // UI elemanlarını config değerleriyle doldur
    ui->addressSpinBox->setValue(config.address);
    ui->nameEdit->setText(config.name);
    ui->descriptionEdit->setText(config.description);
    
    // Data type combo
    int dataTypeIndex = dataTypeMap.value(config.dataType, 0);
    ui->dataTypeCombo->setCurrentIndex(dataTypeIndex);
    
    // Register type combo
    int regTypeIndex = registerTypeMap.value(config.regType, 0);
    ui->registerTypeCombo->setCurrentIndex(regTypeIndex);
    
    // Byte order combo
    int byteOrderIndex = byteOrderMap.value(config.byteOrder, 0);
    ui->byteOrderCombo->setCurrentIndex(byteOrderIndex);
    
    // Scale factor
    ui->scaleFactorSpinBox->setValue(config.scaleFactor);
    
    // Unit
    ui->unitEdit->setText(config.unit);
    
    // Min/Max values
    ui->minValueSpinBox->setValue(config.minValue);
    ui->maxValueSpinBox->setValue(config.maxValue);
    
    // Alarm settings
    ui->alarmEnabledCheck->setChecked(config.isAlarmEnabled);
    ui->alarmLowLimitSpinBox->setValue(config.alarmLowLimit);
    ui->alarmHighLimitSpinBox->setValue(config.alarmHighLimit);
}

ModbusTypes::RegisterConfig RegisterSetupDialog::uiToConfig() const {
    ModbusTypes::RegisterConfig config;
    
    config.address = ui->addressSpinBox->value();
    config.name = ui->nameEdit->text();
    config.description = ui->descriptionEdit->text();
    
    // Data type
    int dataTypeIndex = ui->dataTypeCombo->currentIndex();
    for (auto it = dataTypeMap.begin(); it != dataTypeMap.end(); ++it) {
        if (it.value() == dataTypeIndex) {
            config.dataType = it.key();
            break;
        }
    }
    
    // Register type
    int regTypeIndex = ui->registerTypeCombo->currentIndex();
    for (auto it = registerTypeMap.begin(); it != registerTypeMap.end(); ++it) {
        if (it.value() == regTypeIndex) {
            config.regType = it.key();
            break;
        }
    }
    
    // Byte order
    int byteOrderIndex = ui->byteOrderCombo->currentIndex();
    for (auto it = byteOrderMap.begin(); it != byteOrderMap.end(); ++it) {
        if (it.value() == byteOrderIndex) {
            config.byteOrder = it.key();
            break;
        }
    }
    
    config.scaleFactor = ui->scaleFactorSpinBox->value();
    config.unit = ui->unitEdit->text();
    config.minValue = ui->minValueSpinBox->value();
    config.maxValue = ui->maxValueSpinBox->value();
    config.isAlarmEnabled = ui->alarmEnabledCheck->isChecked();
    config.alarmLowLimit = ui->alarmLowLimitSpinBox->value();
    config.alarmHighLimit = ui->alarmHighLimitSpinBox->value();
    
    return config;
}

void RegisterSetupDialog::updateMinMaxRanges() {
    ModbusTypes::DataType currentType = static_cast<ModbusTypes::DataType>(
        ui->dataTypeCombo->currentData().toInt());
        
    auto it = dataTypeLimits.find(currentType);
    if (it != dataTypeLimits.end()) {
        const auto& limits = it.value();
        
        ui->minValueSpinBox->setRange(limits.minValue, limits.maxValue);
        ui->maxValueSpinBox->setRange(limits.minValue, limits.maxValue);
        ui->alarmLowLimitSpinBox->setRange(limits.minValue, limits.maxValue);
        ui->alarmHighLimitSpinBox->setRange(limits.minValue, limits.maxValue);
    }
}

void RegisterSetupDialog::reject() {
    QDialog::reject();
}