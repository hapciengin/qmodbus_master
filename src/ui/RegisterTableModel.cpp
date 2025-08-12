#include "RegisterTableModel.h"
#include "ModbusRegister.h"
#include <QDateTime>
#include <QColor>
#include <QFont>
#include <QFile>
#include <QTextStream>
#include <QDebug>

RegisterTableModel::RegisterTableModel(QObject* parent)
    : QAbstractTableModel(parent)
    , showScaledValues(true)
    , showHexAddresses(false)
    , showTimestamps(true)
    , highlightAlarms(true)
{
    setupViewConstants();
    
    connect(this, &RegisterTableModel::formatChanged, this, &RegisterTableModel::onFormatChanged);
}

RegisterTableModel::~RegisterTableModel()
{
    clear();
}

QStringList RegisterTableModel::getAvailableFormats()
{
    return {
        "DEC",
        "HEX",
        "BIN",
        "FLOAT",
        "DOUBLE",
        "WORD",
        "DWORD",
        "ASCII"
    };
}

QString RegisterTableModel::formatToString(DisplayFormat format)
{
    switch (format) {
        case DisplayFormat::DEC: return "DEC";
        case DisplayFormat::HEX: return "HEX";
        case DisplayFormat::BIN: return "BIN";
        case DisplayFormat::FLOAT: return "FLOAT";
        case DisplayFormat::DOUBLE: return "DOUBLE";
        case DisplayFormat::WORD: return "WORD";
        case DisplayFormat::DWORD: return "DWORD";
        case DisplayFormat::ASCII: return "ASCII";
        default: return "DEC";
    }
}

RegisterTableModel::DisplayFormat RegisterTableModel::stringToFormat(const QString& str)
{
    if (str == "HEX") return DisplayFormat::HEX;
    if (str == "BIN") return DisplayFormat::BIN;
    if (str == "FLOAT") return DisplayFormat::FLOAT;
    if (str == "DOUBLE") return DisplayFormat::DOUBLE;
    if (str == "WORD") return DisplayFormat::WORD;
    if (str == "DWORD") return DisplayFormat::DWORD;
    if (str == "ASCII") return DisplayFormat::ASCII;
    return DisplayFormat::DEC;  // Default format
}

void RegisterTableModel::setupViewConstants()
{
    viewConst.validColor = QColor(255, 255, 255);  // Beyaz
    viewConst.invalidColor = QColor(255, 240, 240);  // Açık kırmızı
    viewConst.alarmColor = QColor(255, 200, 200);  // Kırmızı
    viewConst.readOnlyColor = QColor(240, 240, 240);  // Gri

    viewConst.normalFont = QFont("Arial", 9);
    viewConst.boldFont = QFont("Arial", 9, QFont::Bold);
    
    viewConst.dateTimeFormat = "yyyy-MM-dd HH:mm:ss.zzz";
}

bool RegisterTableModel::validateRegisterConfig(const ModbusTypes::RegisterConfig& config) const
{
    // Adres kontrolü
    if (config.address < 0 || config.address > 65535) {
        return false;
    }

    // İsim kontrolü
    if (config.name.isEmpty()) {
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

    // Alarm limitleri kontrolü
    if (config.isAlarmEnabled) {
        if (config.alarmLowLimit >= config.alarmHighLimit ||
            config.alarmLowLimit < config.minValue ||
            config.alarmHighLimit > config.maxValue) {
            return false;
        }
    }

    return true;
}

int RegisterTableModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid()) {
        return 0;
    }
    return sortedAddresses.size();
}

int RegisterTableModel::columnCount(const QModelIndex& parent) const
{
    if (parent.isValid()) {
        return 0;
    }
    return Column::COLUMN_COUNT;
}

QVariant RegisterTableModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() >= sortedAddresses.size()) {
        return QVariant();
    }

    int address = sortedAddresses[index.row()];
    const auto& regData = registers[address];
    if (!regData.reg) {
        return QVariant();
    }

    switch (role) {
        case Qt::DisplayRole:
            switch (index.column()) {
                case Column::ADDRESS:
                    return showHexAddresses ? 
                           QString("0x%1").arg(address, 4, 16, QChar('0')).toUpper() :
                           QString::number(address);
                
                case Column::NAME:
                    return regData.reg->getName();
                
                case Column::VALUE:
                {
                    QVariant rawValue = regData.reg->getValue();
                    qDebug() << "VALUE gösteriliyor: adres =" << address << ", değer =" << rawValue << ", format =" << formatToString(regData.format);
                    QString formattedValue = formatValue(rawValue, regData.format);
                    qDebug() << "Formatlanmış değer: " << formattedValue;
                    return formattedValue;
                }
                
                case Column::FORMAT:
                    return formatToString(regData.format);
                
                case Column::TYPE:
                    return getDataTypeString(regData.reg->getDataType());
                
                case Column::ACCESS:
                    return regData.reg->isReadOnly() ? "R" : "R/W";
                
                case Column::SCALE:
                    return QString::number(regData.reg->getScaleFactor(), 'g', 6);
                
                case Column::UNIT:
                    return regData.reg->getUnit();
                
                case Column::MIN_VALUE:
                    return QString::number(regData.reg->getMinValue(), 'g', 6);
                
                case Column::MAX_VALUE:
                    return QString::number(regData.reg->getMaxValue(), 'g', 6);
                
                case Column::DESCRIPTION:
                    return regData.reg->getDescription();
                
                case Column::LAST_UPDATE:
                    return showTimestamps ? 
                           formatDateTime(regData.reg->getLastUpdateTime()) : 
                           QString();
                
                case Column::STATUS:
                    return getStatusString(regData.reg.get());
            }
            break;

        case Qt::EditRole:
            switch (index.column()) {
                case Column::VALUE:
                    return regData.reg->getValue();
                
                case Column::FORMAT:
                    return formatToString(regData.format);
                
                case Column::NAME:
                case Column::DESCRIPTION:
                case Column::UNIT:
                case Column::SCALE:
                case Column::MIN_VALUE:
                case Column::MAX_VALUE:
                    return data(index, Qt::DisplayRole);
                
                default:
                    return QVariant();
            }
            break;

        case Qt::TextAlignmentRole:
            switch (index.column()) {
                case Column::ADDRESS:
                case Column::VALUE:
                case Column::SCALE:
                case Column::MIN_VALUE:
                case Column::MAX_VALUE:
                    return int(Qt::AlignRight | Qt::AlignVCenter);
                
                case Column::TYPE:
                case Column::ACCESS:
                case Column::UNIT:
                case Column::STATUS:
                    return int(Qt::AlignCenter);
                
                default:
                    return int(Qt::AlignLeft | Qt::AlignVCenter);
            }
            break;

        case Qt::BackgroundRole:
            if (!regData.reg->isValid() && index.column() == Column::VALUE) {
                return viewConst.invalidColor;
            }
            if (highlightAlarms && regData.reg->isInAlarmState()) {
                return viewConst.alarmColor;
            }
            if (regData.reg->isReadOnly() && index.column() == Column::VALUE) {
                return viewConst.readOnlyColor;
            }
            return viewConst.validColor;

        case Qt::ForegroundRole:
            if (!regData.reg->isValid() && index.column() == Column::VALUE) {
                return QColor(Qt::red);
            }
            if (regData.reg->isInAlarmState()) {
                return QColor(Qt::darkRed);
            }
            return QColor(Qt::black);

        case Qt::FontRole:
            if (index.column() == Column::ADDRESS || 
                (regData.reg->isInAlarmState() && index.column() == Column::VALUE)) {
                return viewConst.boldFont;
            }
            return viewConst.normalFont;

        case Qt::ToolTipRole:
            switch (index.column()) {
                case Column::VALUE:
                    return QString("Raw: %1\nScaled: %2")
                            .arg(regData.reg->getValue().toString())
                            .arg(regData.reg->getScaledValue().toString());
                
                case Column::STATUS:
                    if (!regData.reg->isValid()) {
                        return tr("Invalid value");
                    }
                    if (regData.reg->isInAlarmState()) {
                        return tr("Alarm: Value out of range");
                    }
                    return tr("Valid");
                
                default:
                    return data(index, Qt::DisplayRole);
            }
            break;
    }

    return QVariant();
}


QVariant RegisterTableModel::getDisplayData(const ModbusRegister* reg, int column) const
{
    switch (column) {
        case Column::ADDRESS:
            return formatAddress(reg->getAddress());
            
        case Column::NAME:
            return reg->getName();
            
        case Column::VALUE:
            return formatValue(reg);
            
        case Column::TYPE:
            return getDataTypeString(reg->getDataType());
            
        case Column::ACCESS:
            return reg->isReadOnly() ? "R" : "R/W";
            
        case Column::SCALE:
            return QString::number(reg->getScaleFactor(), 'g', 6);
            
        case Column::UNIT:
            return reg->getUnit();
            
        case Column::MIN_VALUE:
            return QString::number(reg->getMinValue(), 'g', 6);
            
        case Column::MAX_VALUE:
            return QString::number(reg->getMaxValue(), 'g', 6);
            
        case Column::DESCRIPTION:
            return reg->getDescription();
            
        case Column::LAST_UPDATE:
            return showTimestamps ? formatDateTime(reg->getLastUpdateTime()) : "";
            
        case Column::STATUS:
            return getStatusString(reg);
            
        default:
            return QVariant();
    }
}

QVariant RegisterTableModel::getEditData(const ModbusRegister* reg, int column) const
{
    switch (column) {
        case Column::VALUE:
            return showScaledValues ? reg->getScaledValue() : reg->getValue();
            
        case Column::NAME:
        case Column::DESCRIPTION:
        case Column::UNIT:
            return data(index(addressToRow(reg->getAddress()), column), Qt::DisplayRole);
            
        case Column::SCALE:
            return reg->getScaleFactor();
            
        case Column::MIN_VALUE:
            return reg->getMinValue();
            
        case Column::MAX_VALUE:
            return reg->getMaxValue();
            
        default:
            return QVariant();
    }
}

QVariant RegisterTableModel::getToolTipData(const ModbusRegister* reg, int column) const
{
    switch (column) {
        case Column::VALUE:
            return QString("Raw: %1\nScaled: %2")
                .arg(reg->getValue().toString())
                .arg(reg->getScaledValue().toString());
            
        case Column::STATUS:
            if (!reg->isValid()) {
                return tr("Invalid value");
            }
            if (reg->isInAlarmState()) {
                return tr("Alarm: Value out of range");
            }
            return tr("Valid");
            
        default:
            return data(index(addressToRow(reg->getAddress()), column), Qt::DisplayRole);
    }
}

QVariant RegisterTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
            case Column::ADDRESS:
                return tr("Address");
            case Column::NAME:
                return tr("Name");
            case Column::VALUE:
                return tr("Value");
            case Column::TYPE:
                return tr("Type");
            case Column::ACCESS:
                return tr("Access");
            case Column::SCALE:
                return tr("Scale");
            case Column::UNIT:
                return tr("Unit");
            case Column::MIN_VALUE:
                return tr("Min");
            case Column::MAX_VALUE:
                return tr("Max");
            case Column::DESCRIPTION:
                return tr("Description");
            case Column::LAST_UPDATE:
                return tr("Last Update");
            case Column::STATUS:
                return tr("Status");
            default:
                return QVariant();
        }
    }
    
    if (orientation == Qt::Vertical && role == Qt::DisplayRole) {
        return section + 1;
    }
    
    return QVariant();
}

Qt::ItemFlags RegisterTableModel::flags(const QModelIndex& index) const
{
    if (!index.isValid()) {
        return Qt::NoItemFlags;
    }

    Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;

    // Düzenlenebilir sütunlar
    switch (index.column()) {
        case Column::VALUE: {
            int address = sortedAddresses[index.row()];
            const auto& regData = registers[address];
            if (regData.reg && !regData.reg->isReadOnly()) {
                flags |= Qt::ItemIsEditable;
            }
            break;
        }
        case Column::FORMAT:  // FORMAT sütununu düzenlenebilir yap
        case Column::NAME:
        case Column::DESCRIPTION:
        case Column::UNIT:
        case Column::SCALE:
        case Column::MIN_VALUE:
        case Column::MAX_VALUE:
            flags |= Qt::ItemIsEditable;
            break;
    }

    return flags;
}

bool RegisterTableModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (!index.isValid() || role != Qt::EditRole) {
        return false;
    }

    int address = sortedAddresses[index.row()];
    auto& regData = registers[address];
    
    if (!regData.reg) {
        return false;
    }

    bool success = false;

    switch (index.column()) {
        case Column::NAME:
            {
                auto config = regData.reg->getConfig();
                config.name = value.toString();
                regData.reg->updateConfig(config);
                success = true;
            }
            break;

        case Column::VALUE:
            {
                if (!regData.reg->isReadOnly()) {
                    QVariant parsedValue;
                    bool ok = true;
                    
                    switch (regData.format) {
                        case DisplayFormat::HEX:
                            {
                                QString hexStr = value.toString();
                                if (hexStr.startsWith("0x", Qt::CaseInsensitive)) {
                                    hexStr.remove(0, 2);
                                }
                                parsedValue = hexStr.toUInt(&ok, 16);
                            }
                            break;
                            
                        case DisplayFormat::BIN:
                            {
                                QString binStr = value.toString();
                                if (binStr.startsWith("0b", Qt::CaseInsensitive)) {
                                    binStr.remove(0, 2);
                                }
                                parsedValue = binStr.toUInt(&ok, 2);
                            }
                            break;
                            
                        case DisplayFormat::FLOAT:
                            {
                                float fVal = value.toFloat(&ok);
                                if (ok) {
                                    quint32 rawValue = *reinterpret_cast<quint32*>(&fVal);
                                    parsedValue = rawValue;
                                }
                            }
                            break;
                            
                        case DisplayFormat::DOUBLE:
                            {
                                double dVal = value.toDouble(&ok);
                                if (ok) {
                                    quint64 rawValue = *reinterpret_cast<quint64*>(&dVal);
                                    parsedValue = static_cast<quint32>(rawValue & 0xFFFFFFFF);
                                }
                            }
                            break;
                            
                        case DisplayFormat::WORD:
                            {
                                QString hexStr = value.toString();
                                if (hexStr.startsWith("0x", Qt::CaseInsensitive)) {
                                    hexStr.remove(0, 2);
                                }
                                parsedValue = hexStr.toUInt(&ok, 16) & 0xFFFF;
                            }
                            break;
                            
                        case DisplayFormat::DWORD:
                            {
                                QString hexStr = value.toString();
                                if (hexStr.startsWith("0x", Qt::CaseInsensitive)) {
                                    hexStr.remove(0, 2);
                                }
                                parsedValue = hexStr.toUInt(&ok, 16);
                            }
                            break;
                            
                        case DisplayFormat::ASCII:
                            {
                                QString asciiStr = value.toString();
                                if (asciiStr.length() > 0) {
                                    quint16 rawValue = 0;
                                    if (asciiStr.length() >= 1) {
                                        rawValue |= (static_cast<quint16>(asciiStr[0].toLatin1()) << 8);
                                    }
                                    if (asciiStr.length() >= 2) {
                                        rawValue |= static_cast<quint16>(asciiStr[1].toLatin1());
                                    }
                                    parsedValue = rawValue;
                                } else {
                                    parsedValue = 0;
                                }
                            }
                            break;
                            
                        case DisplayFormat::DEC:
                        default:
                            parsedValue = value.toInt(&ok);
                            break;
                    }
                    
                    if (ok) {
                        success = regData.reg->setValue(parsedValue);
                    }
                }
            }
            break;

        case Column::FORMAT:
            {
                DisplayFormat newFormat = stringToFormat(value.toString());
                qDebug() << "Format değişiyor: " << formatToString(regData.format) << " -> " << formatToString(newFormat);
                
                if (regData.format != newFormat) {
                    regData.format = newFormat;
                    success = true;
                    
                    // Format değiştiğinde VALUE sütununu güncelle
                    emit dataChanged(index.sibling(index.row(), Column::VALUE), index.sibling(index.row(), Column::VALUE));
                    emit formatChanged(address, newFormat);
                }
            }
            break;

        case Column::DESCRIPTION:
            {
                auto config = regData.reg->getConfig();
                config.description = value.toString();
                regData.reg->updateConfig(config);
                success = true;
            }
            break;

        case Column::SCALE:
            {
                bool ok;
                double scale = value.toDouble(&ok);
                if (ok && scale != 0.0) {
                    auto config = regData.reg->getConfig();
                    config.scaleFactor = scale;
                    regData.reg->updateConfig(config);
                    success = true;
                }
            }
            break;

        case Column::UNIT:
            {
                auto config = regData.reg->getConfig();
                config.unit = value.toString();
                regData.reg->updateConfig(config);
                success = true;
            }
            break;

        case Column::MIN_VALUE:
            {
                bool ok;
                double minVal = value.toDouble(&ok);
                if (ok && minVal < regData.reg->getMaxValue()) {
                    auto config = regData.reg->getConfig();
                    config.minValue = minVal;
                    regData.reg->updateConfig(config);
                    success = true;
                }
            }
            break;

        case Column::MAX_VALUE:
            {
                bool ok;
                double maxVal = value.toDouble(&ok);
                if (ok && maxVal > regData.reg->getMinValue()) {
                    auto config = regData.reg->getConfig();
                    config.maxValue = maxVal;
                    regData.reg->updateConfig(config);
                    success = true;
                }
            }
            break;
    }

    if (success) {
        emit dataChanged(index, index);
        emit registerUpdated(address);
    }

    return success;
}

bool RegisterTableModel::addRegister(const ModbusTypes::RegisterConfig& config)
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
    
    RegisterData regData;
    regData.reg = std::make_shared<ModbusRegister>(config);
    regData.format = DisplayFormat::DEC;
    registers[config.address] = regData;
    
    connect(regData.reg.get(), &ModbusRegister::valueChanged,
            this, [this, address = config.address](const QVariant& value) {
                emit valueChanged(address, value);  // registerValueChanged yerine valueChanged kullanıyoruz
            });
    
    sortedAddresses.append(config.address);
    std::sort(sortedAddresses.begin(), sortedAddresses.end());
    
    emit registerAdded(config.address);
    emit configurationChanged();
    
    return true;
}

bool RegisterTableModel::removeRegister(int address)
{
    if (!registers.contains(address)) {
        return false;
    }

    int row = addressToRow(address);
    if (row < 0) {
        return false;
    }

    beginRemoveRows(QModelIndex(), row, row);
    registers.remove(address);
    sortedAddresses.removeOne(address);
    endRemoveRows();

    emit registerRemoved(address);
    return true;
}

bool RegisterTableModel::updateRegister(int address, const ModbusTypes::RegisterConfig& config)
{
    QMutexLocker locker(&registerMutex);
    
    if (!registers.contains(address)) {
        lastError = "Register does not exist";
        return false;
    }
    
    auto& regData = registers[address];
    if (!regData.reg) {
        lastError = "Invalid register";
        return false;
    }
    
    regData.reg->updateConfig(config);
    emit registerUpdated(address);
    emit configurationChanged();
    
    return true;
}

ModbusTypes::RegisterConfig RegisterTableModel::getRegisterConfig(int address) const
{
    QMutexLocker locker(&registerMutex);
    
    if (registers.contains(address) && registers[address].reg) {
        return registers[address].reg->getConfig();
    }
    
    return ModbusTypes::RegisterConfig();
}

QVector<int> RegisterTableModel::getRegisterAddresses() const
{
    QMutexLocker locker(&registerMutex);
    return sortedAddresses;
}

bool RegisterTableModel::setValue(int address, const QVariant& value)
{
    QMutexLocker locker(&registerMutex);
    
    if (!registers.contains(address)) {
        lastError = "Register does not exist";
        return false;
    }
    
    auto& regData = registers[address];
    if (!regData.reg) {
        lastError = "Invalid register";
        return false;
    }
    
    if (regData.reg->setValue(value)) {
        int row = addressToRow(address);
        if (row >= 0) {
            emit dataChanged(index(row, Column::VALUE), index(row, Column::STATUS));
        }
        return true;
    }
    
    return false;
}

QVariant RegisterTableModel::getValue(int address) const
{
    QMutexLocker locker(&registerMutex);
    
    if (registers.contains(address) && registers[address].reg) {
        return showScaledValues ? 
               registers[address].reg->getScaledValue() : 
               registers[address].reg->getValue();
    }
    return QVariant();
}

bool RegisterTableModel::isValid(int address) const
{
    QMutexLocker locker(&registerMutex);
    
    if (registers.contains(address) && registers[address].reg) {
        return registers[address].reg->isValid();
    }
    return false;
}

void RegisterTableModel::clear()
{
    if (!registers.isEmpty()) {
        beginResetModel();
        registers.clear();
        sortedAddresses.clear();
        endResetModel();
    }
}

void RegisterTableModel::updateAll()
{
    if (!registers.isEmpty()) {
        emit dataChanged(index(0, 0), 
                        index(sortedAddresses.size() - 1, COLUMN_COUNT - 1));
    }
}

void RegisterTableModel::invalidateAll()
{
    for (auto& regData : registers) {
        if (regData.reg) {
            regData.reg->setValue(QVariant());
            regData.reg->updateConfig(regData.reg->getConfig());
        }
    }
    updateAll();
}

void RegisterTableModel::setShowScaledValues(bool show)
{
    if (showScaledValues != show) {
        showScaledValues = show;
        // Sadece değer sütununu güncelle
        if (!registers.isEmpty()) {
            emit dataChanged(index(0, Column::VALUE),
                           index(sortedAddresses.size() - 1, Column::VALUE));
        }
    }
}

void RegisterTableModel::setShowHexAddresses(bool show)
{
    if (showHexAddresses != show) {
        showHexAddresses = show;
        // Sadece adres sütununu güncelle
        if (!registers.isEmpty()) {
            emit dataChanged(index(0, Column::ADDRESS),
                           index(sortedAddresses.size() - 1, Column::ADDRESS));
        }
    }
}

void RegisterTableModel::setShowTimestamps(bool show)
{
    if (showTimestamps != show) {
        showTimestamps = show;
        // Sadece timestamp sütununu güncelle
        if (!registers.isEmpty()) {
            emit dataChanged(index(0, Column::LAST_UPDATE),
                           index(sortedAddresses.size() - 1, Column::LAST_UPDATE));
        }
    }
}

void RegisterTableModel::setHighlightAlarms(bool highlight)
{
    if (highlightAlarms != highlight) {
        highlightAlarms = highlight;
        // Tüm tabloyu güncelle çünkü arka plan renkleri değişecek
        if (!registers.isEmpty()) {
            emit dataChanged(index(0, 0),
                           index(sortedAddresses.size() - 1, COLUMN_COUNT - 1));
        }
    }
}

bool RegisterTableModel::importFromCsv(const QString& filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        emit error(tr("Cannot open file: %1").arg(filename));
        return false;
    }

    QTextStream in(&file);
    
    // Header'ı oku ve doğrula
    QString headerLine = in.readLine();
    QStringList expectedHeaders = prepareCsvHeader();
    QStringList actualHeaders = headerLine.split(',');
    
    if (actualHeaders != expectedHeaders) {
        emit error(tr("Invalid CSV header format"));
        return false;
    }

    // Mevcut register'ları temizle
    clear();

    // Satır satır oku ve register'ları ekle
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(',');
        
        if (!parseCsvRecord(fields)) {
            emit error(tr("Error parsing CSV record: %1").arg(line));
            continue;
        }
    }

    return true;
}

bool RegisterTableModel::exportToCsv(const QString& filename) const
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly)) {
        // const_cast kullanarak const'u kaldır
        const_cast<RegisterTableModel*>(this)->error(
            tr("Cannot open file for writing: %1").arg(filename));
        return false;
    }

    QTextStream out(&file);

    // Header'ı yaz
    QStringList headers = prepareCsvHeader();
    out << headers.join(',') << "\n";

    // Her register için bir satır yaz
    for (int address : sortedAddresses) {
        QStringList record = prepareCsvRecord(address);
        out << record.join(',') << "\n";
    }

    return true;
}

void RegisterTableModel::onFormatChanged(int address, DisplayFormat format)
{
    qDebug() << "onFormatChanged: adres =" << address << ", format =" << formatToString(format);
    
    int row = addressToRow(address);
    if (row >= 0) {
        emit dataChanged(index(row, Column::VALUE), index(row, Column::VALUE));
    }
}

void RegisterTableModel::onRegisterAlarmStateChanged(int address, bool inAlarm)
{
    int row = addressToRow(address);
    if (row >= 0) {
        // Durum sütununu ve muhtemelen arka plan rengini güncelle
        emit dataChanged(index(row, 0),
                        index(row, COLUMN_COUNT - 1));
        emit alarmStateChanged(address, inAlarm);
    }
}

void RegisterTableModel::onRegisterValueChanged(int address, const QVariant& value)
{
    int row = addressToRow(address);
    if (row >= 0) {
        // Değer ve durum sütunlarını güncelle
        emit dataChanged(index(row, Column::VALUE),
                        index(row, Column::STATUS));
        emit valueChanged(address, value);
    }
}

void RegisterTableModel::onRegisterConfigChanged(int address)
{
    int row = addressToRow(address);
    if (row >= 0) {
        // Tüm sütunları güncelle çünkü herhangi bir özellik değişmiş olabilir
        emit dataChanged(index(row, 0),
                        index(row, COLUMN_COUNT - 1));
        emit registerUpdated(address);
    }
}

QStringList RegisterTableModel::prepareCsvHeader() const
{
    return QStringList() 
        << "Address" << "Name" << "Type" << "Access" 
        << "Scale" << "Unit" << "MinValue" << "MaxValue" 
        << "Description" << "AlarmEnabled" << "AlarmLowLimit" 
        << "AlarmHighLimit" << "ByteOrder";
}

QStringList RegisterTableModel::prepareCsvRecord(int address) const
{
    QMutexLocker locker(&registerMutex);
    
    const auto& regData = registers[address];
    if (!regData.reg) {
        return QStringList();
    }

    const auto& config = regData.reg->getConfig();
    return QStringList()
        << QString::number(config.address)
        << config.name
        << QString::number(static_cast<int>(config.dataType))
        << (config.isReadOnly ? "R" : "RW")
        << QString::number(config.scaleFactor)
        << config.unit
        << QString::number(config.minValue)
        << QString::number(config.maxValue)
        << config.description
        << (config.isAlarmEnabled ? "1" : "0")
        << QString::number(config.alarmLowLimit)
        << QString::number(config.alarmHighLimit)
        << QString::number(static_cast<int>(config.byteOrder));
}

bool RegisterTableModel::parseCsvRecord(const QStringList& fields)
{
    if (fields.size() < 13) return false;

    ModbusTypes::RegisterConfig config;
    bool ok;

    // Temel alanları parse et
    config.address = fields[0].toInt(&ok);
    if (!ok) return false;

    config.name = fields[1];
    config.dataType = static_cast<ModbusTypes::DataType>(fields[2].toInt(&ok));
    if (!ok) return false;

    config.isReadOnly = (fields[3] == "R");
    
    config.scaleFactor = fields[4].toDouble(&ok);
    if (!ok) return false;

    config.unit = fields[5];
    
    config.minValue = fields[6].toDouble(&ok);
    if (!ok) return false;

    config.maxValue = fields[7].toDouble(&ok);
    if (!ok) return false;

    config.description = fields[8];
    
    config.isAlarmEnabled = (fields[9] == "1");
    
    config.alarmLowLimit = fields[10].toDouble(&ok);
    if (!ok) return false;

    config.alarmHighLimit = fields[11].toDouble(&ok);
    if (!ok) return false;

    config.byteOrder = static_cast<ModbusTypes::ByteOrder>(fields[12].toInt(&ok));
    if (!ok) return false;

    // Register'ı ekle
    return addRegister(config);
}

int RegisterTableModel::addressToRow(int address) const
{
    return sortedAddresses.indexOf(address);
}

int RegisterTableModel::rowToAddress(int row) const
{
    if (row >= 0 && row < sortedAddresses.size()) {
        return sortedAddresses[row];
    }
    return -1;
}

QString RegisterTableModel::formatValue(const ModbusRegister* reg) const
{
    if (!reg->isValid()) {
        return "---";
    }

    QVariant value = showScaledValues ? reg->getScaledValue() : reg->getValue();
    
    switch (reg->getDataType()) {
        case ModbusTypes::DataType::BIT:
            return value.toBool() ? "1" : "0";
            
        case ModbusTypes::DataType::BYTE:
        case ModbusTypes::DataType::WORD:
        case ModbusTypes::DataType::DWORD:
            return QString("0x%1").arg(value.toUInt(), 
                    (reg->getDataType() == ModbusTypes::DataType::BYTE ? 2 :
                     reg->getDataType() == ModbusTypes::DataType::WORD ? 4 : 8), 
                    16, QChar('0')).toUpper();
            
        case ModbusTypes::DataType::INT:
        case ModbusTypes::DataType::DINT:
            return QString::number(value.toInt());
            
        case ModbusTypes::DataType::REAL:
            return QString::number(value.toFloat(), 'f', 3);
            
        case ModbusTypes::DataType::LREAL:
            return QString::number(value.toDouble(), 'f', 6);
            
        case ModbusTypes::DataType::STRING:
        case ModbusTypes::DataType::WSTRING:
            return value.toString();
            
        default:
            return value.toString();
    }
}

QString RegisterTableModel::formatAddress(int address) const
{
    return showHexAddresses ? 
           QString("0x%1").arg(address, 4, 16, QChar('0')).toUpper() :
           QString::number(address);
}

QString RegisterTableModel::formatDateTime(const QDateTime& dt) const
{
    return dt.isValid() ? dt.toString(viewConst.dateTimeFormat) : "---";
}

QString RegisterTableModel::getDataTypeString(ModbusTypes::DataType type) const
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

QString RegisterTableModel::getStatusString(const ModbusRegister* reg) const
{
    if (!reg->isValid()) {
        return tr("Invalid");
    }
    if (reg->isInAlarmState()) {
        return tr("Alarm");
    }
    return tr("Valid");
}

QVariant RegisterTableModel::getCellBackgroundColor(const ModbusRegister* reg, int column) const
{
    // Alarm durumunda ve highlight aktifse kırmızı arka plan
    if (highlightAlarms && reg->isInAlarmState() && column == Column::VALUE) {
        return viewConst.alarmColor;
    }
    
    // Geçersiz değer için açık kırmızı arka plan
    if (!reg->isValid() && column == Column::VALUE) {
        return viewConst.invalidColor;
    }
    
    // Salt okunur hücreler için gri arka plan
    if (reg->isReadOnly() && column == Column::VALUE) {
        return viewConst.readOnlyColor;
    }
    
    return viewConst.validColor;
}

QVariant RegisterTableModel::getCellForegroundColor(const ModbusRegister* reg, int column) const
{
    // Alarm durumunda koyu kırmızı yazı
    if (reg->isInAlarmState() && column == Column::VALUE) {
        return QColor(Qt::darkRed);
    }
    
    // Geçersiz değer için kırmızı yazı
    if (!reg->isValid() && column == Column::VALUE) {
        return QColor(Qt::red);
    }
    
    return QColor(Qt::black);
}

QVariant RegisterTableModel::getCellFont(const ModbusRegister* reg, int column) const
{
    // Alarm durumunda kalın yazı
    if (reg->isInAlarmState() && column == Column::VALUE) {
        return viewConst.boldFont;
    }
    
    // Adres sütunu için kalın yazı
    if (column == Column::ADDRESS) {
        return viewConst.boldFont;
    }
    
    return viewConst.normalFont;
}

QVariant RegisterTableModel::getAlignmentForColumn(int column) const
{
    switch (column) {
        case Column::ADDRESS:
        case Column::VALUE:
        case Column::SCALE:
        case Column::MIN_VALUE:
        case Column::MAX_VALUE:
            return Qt::AlignRight + Qt::AlignVCenter;
            
        case Column::TYPE:
        case Column::ACCESS:
        case Column::UNIT:
        case Column::STATUS:
            return Qt::AlignCenter;
            
        default:
            return Qt::AlignLeft + Qt::AlignVCenter;
    }
}

bool RegisterTableModel::validateCellEdit(const ModbusRegister* reg, int column, const QVariant& value) const
{
    switch (column) {
        case Column::VALUE:
            return reg->validateValue(value);
            
        case Column::SCALE:
            {
                bool ok;
                double scale = value.toDouble(&ok);
                return ok && scale != 0.0;
            }
            
        case Column::MIN_VALUE:
            {
                bool ok;
                double min = value.toDouble(&ok);
                return ok && min < reg->getMaxValue();
            }
            
        case Column::MAX_VALUE:
            {
                bool ok;
                double max = value.toDouble(&ok);
                return ok && max > reg->getMinValue();
            }
            
        case Column::NAME:
        case Column::DESCRIPTION:
        case Column::UNIT:
            return !value.toString().isEmpty();
            
        default:
            return false;
    }
}

QString RegisterTableModel::formatValue(const QVariant& value, DisplayFormat format) const
{
    if (!value.isValid()) {
        return "---";
    }

    bool ok;
    quint32 rawValue = value.toUInt(&ok);
    if (!ok) {
        return value.toString();
    }

    switch (format) {
        case DisplayFormat::HEX:
            return QString("0x%1").arg(rawValue, 4, 16, QChar('0')).toUpper();
            
        case DisplayFormat::BIN:
            return QString("0b%1").arg(rawValue, 16, 2, QChar('0'));
            
        case DisplayFormat::FLOAT:
            {
                float fVal = *reinterpret_cast<float*>(&rawValue);
                return QString::number(fVal, 'f', 3);
            }
            
        case DisplayFormat::DOUBLE:
            {
                double dVal = *reinterpret_cast<double*>(&rawValue);
                return QString::number(dVal, 'f', 6);
            }
            
        case DisplayFormat::WORD:
            return QString("0x%1").arg(rawValue & 0xFFFF, 4, 16, QChar('0')).toUpper();
            
        case DisplayFormat::DWORD:
            return QString("0x%1").arg(rawValue, 8, 16, QChar('0')).toUpper();
            
        case DisplayFormat::ASCII:
            {
                QString asciiResult;
                QByteArray bytes;
                bytes.append((rawValue >> 8) & 0xFF);
                bytes.append(rawValue & 0xFF);
                for (char c : bytes) {
                    if (c >= 32 && c <= 126) {
                        asciiResult += QChar(c);
                    } else {
                        asciiResult += '.';
                    }
                }
                return asciiResult;
            }
            
        case DisplayFormat::DEC:
        default:
            return QString::number(rawValue);
    }
}

QString RegisterTableModel::decimalToHex(const QVariant& value) const
{
    bool ok;
    uint val = value.toUInt(&ok);
    if (!ok) return "---";
    return QString("0x%1").arg(val, 4, 16, QChar('0')).toUpper();
}

QString RegisterTableModel::decimalToBinary(const QVariant& value) const
{
    bool ok;
    uint val = value.toUInt(&ok);
    if (!ok) return "---";
    return QString("0b%1").arg(val, 16, 2, QChar('0'));
}

QString RegisterTableModel::formatFloat(double value, int precision) const
{
    return QString::number(value, 'f', precision);
}

QString RegisterTableModel::formatWord(quint16 value) const
{
    return QString("0x%1").arg(value, 4, 16, QChar('0')).toUpper();
}

QString RegisterTableModel::formatDWord(quint32 value) const
{
    return QString("0x%1").arg(value, 8, 16, QChar('0')).toUpper();
}

QString RegisterTableModel::formatAscii(const QByteArray& data) const
{
    QString result;
    for (char c : data) {
        if (c >= 32 && c <= 126) {
            result += c;
        } else {
            result += '.';
        }
    }
    return result;
}


RegisterTableModel::DisplayFormat RegisterTableModel::getRegisterFormat(int address) const
{
    QMutexLocker locker(&registerMutex);
    if (registers.contains(address)) {
        return registers[address].format;
    }
    return DisplayFormat::DEC;
}

bool RegisterTableModel::setRegisterFormat(int address, DisplayFormat format)
{
    QMutexLocker locker(&registerMutex);
    if (!registers.contains(address)) {
        return false;
    }
    
    auto& regData = registers[address];
    if (regData.format != format) {
        regData.format = format;
        int row = addressToRow(address);
        if (row >= 0) {
            emit dataChanged(index(row, Column::FORMAT), index(row, Column::FORMAT));
            emit formatChanged(address, format);
        }
    }
    return true;
}