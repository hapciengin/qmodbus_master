#include "ModbusRegister.h"
#include <QDebug>
#include <QDataStream>

ModbusRegister::ModbusRegister(const ModbusTypes::RegisterConfig& config, QObject* parent)
    : QObject(parent)
    , config(config)
    , valid(false)
    , alarmState(false)
    , updateCount(0)
{
    // İlk değer ataması
    switch (config.dataType) {
        case ModbusTypes::DataType::BIT:
            value = false;
            break;
        case ModbusTypes::DataType::BYTE:
        case ModbusTypes::DataType::WORD:
            value = static_cast<quint16>(0);
            break;
        case ModbusTypes::DataType::INT:
            value = static_cast<qint16>(0);
            break;
        case ModbusTypes::DataType::DWORD:
            value = static_cast<quint32>(0);
            break;
        case ModbusTypes::DataType::DINT:
            value = static_cast<qint32>(0);
            break;
        case ModbusTypes::DataType::REAL:
            value = 0.0f;
            break;
        case ModbusTypes::DataType::LREAL:
            value = 0.0;
            break;
        case ModbusTypes::DataType::STRING:
        case ModbusTypes::DataType::WSTRING:
            value = QString();
            break;
    }
}

ModbusRegister::~ModbusRegister()
{
}

QVariant ModbusRegister::getValue() const
{
    QMutexLocker locker(&mutex);
    qDebug() << "ModbusRegister::getValue: adres =" << config.address 
             << ", değer =" << value 
             << ", tip =" << value.typeName();
    return value;
}

bool ModbusRegister::setValue(const QVariant& newValue)
{
    QMutexLocker locker(&mutex);
    
    if (config.isReadOnly) {
        emit error(tr("Cannot write to read-only register"));
        return false;
    }
    
    if (!validateValue(newValue)) {
        emit error(tr("Invalid value for register"));
        return false;
    }
    
    QVariant convertedValue;
    if (!convertValue(newValue, convertedValue)) {
        emit error(tr("Value conversion failed"));
        return false;
    }
    
    if (value != convertedValue) {
        value = convertedValue;
        valid = true;
        lastUpdateTime = QDateTime::currentDateTime();
        updateCount++;
        
        checkAlarmState();
        
        emit valueChanged(value);
        emit scaledValueChanged(getScaledValue());
    }
    
    return true;
}

QString ModbusRegister::getFormattedValue() const
{
    QMutexLocker locker(&mutex);
    return formatValue(value);
}

QByteArray ModbusRegister::getRawData() const
{
    QMutexLocker locker(&mutex);
    return convertToRawData(value);
}

bool ModbusRegister::setRawData(const QByteArray& data)
{
    QMutexLocker locker(&mutex);
    
    if (config.isReadOnly) {
        emit error(tr("Cannot write to read-only register"));
        return false;
    }
    
    QVariant convertedValue = convertFromRawData(data);
    if (!convertedValue.isValid()) {
        emit error(tr("Failed to convert raw data"));
        return false;
    }
    
    if (value != convertedValue) {
        value = convertedValue;
        valid = true;
        lastUpdateTime = QDateTime::currentDateTime();
        updateCount++;
        
        checkAlarmState();
        
        emit valueChanged(value);
        emit scaledValueChanged(getScaledValue());
    }
    
    return true;
}

void ModbusRegister::setScaleFactor(double factor)
{
    if (factor == 0.0) {
        emit error(tr("Scale factor cannot be zero"));
        return;
    }
    
    QMutexLocker locker(&mutex);
    if (config.scaleFactor != factor) {
        config.scaleFactor = factor;
        emit configChanged();
        emit scaledValueChanged(getScaledValue());
    }
}

void ModbusRegister::setUnit(const QString& unit)
{
    QMutexLocker locker(&mutex);
    if (config.unit != unit) {
        config.unit = unit;
        emit configChanged();
    }
}

QVariant ModbusRegister::getScaledValue() const
{
    QMutexLocker locker(&mutex);
    return scaleValue(value, false);
}

bool ModbusRegister::setScaledValue(const QVariant& scaledValue)
{
    QMutexLocker locker(&mutex);
    return setValue(scaleValue(scaledValue, true));
}

void ModbusRegister::setMinValue(double min)
{
    QMutexLocker locker(&mutex);
    if (config.minValue != min) {
        config.minValue = min;
        
        // Alarm limitleri kontrolü
        if (config.isAlarmEnabled && config.alarmLowLimit < min) {
            config.alarmLowLimit = min;
        }
        
        emit configChanged();
        checkAlarmState();
    }
}

void ModbusRegister::setMaxValue(double max)
{
    QMutexLocker locker(&mutex);
    if (config.maxValue != max) {
        config.maxValue = max;
        
        // Alarm limitleri kontrolü
        if (config.isAlarmEnabled && config.alarmHighLimit > max) {
            config.alarmHighLimit = max;
        }
        
        emit configChanged();
        checkAlarmState();
    }
}

bool ModbusRegister::isValueInRange(const QVariant& testValue) const
{
    QMutexLocker locker(&mutex);
    
    bool ok;
    double numValue = testValue.toDouble(&ok);
    if (!ok) return false;
    
    return (numValue >= config.minValue && numValue <= config.maxValue);
}

void ModbusRegister::setAlarmEnabled(bool enabled)
{
    QMutexLocker locker(&mutex);
    if (config.isAlarmEnabled != enabled) {
        config.isAlarmEnabled = enabled;
        emit configChanged();
        checkAlarmState();
    }
}

void ModbusRegister::setAlarmHighLimit(double limit)
{
    QMutexLocker locker(&mutex);
    if (config.alarmHighLimit != limit) {
        // Limit kontrolü
        if (limit > config.maxValue) {
            limit = config.maxValue;
        }
        if (limit <= config.alarmLowLimit) {
            emit error(tr("High alarm limit must be greater than low alarm limit"));
            return;
        }
        
        config.alarmHighLimit = limit;
        emit configChanged();
        checkAlarmState();
    }
}

void ModbusRegister::setAlarmLowLimit(double limit)
{
    QMutexLocker locker(&mutex);
    if (config.alarmLowLimit != limit) {
        // Limit kontrolü
        if (limit < config.minValue) {
            limit = config.minValue;
        }
        if (limit >= config.alarmHighLimit) {
            emit error(tr("Low alarm limit must be less than high alarm limit"));
            return;
        }
        
        config.alarmLowLimit = limit;
        emit configChanged();
        checkAlarmState();
    }
}

void ModbusRegister::setReadOnly(bool readonly)
{
    QMutexLocker locker(&mutex);
    if (config.isReadOnly != readonly) {
        config.isReadOnly = readonly;
        emit configChanged();
    }
}

void ModbusRegister::updateConfig(const ModbusTypes::RegisterConfig& newConfig)
{
    QMutexLocker locker(&mutex);
    
    // Yeni bir kopya oluştur ve değişmemesi gereken özellikleri koru
    ModbusTypes::RegisterConfig updatedConfig = newConfig;
    updatedConfig.address = config.address;
    updatedConfig.dataType = config.dataType;
    updatedConfig.regType = config.regType;
    
    // Yeni yapılandırmayı uygula
    config = updatedConfig;
    
    // Mevcut değeri yeni yapılandırmaya göre kontrol et
    QVariant oldValue = value;
    if (!validateValue(oldValue)) {
        value = QVariant();
        valid = false;
        emit valueChanged(value);
        emit scaledValueChanged(getScaledValue());
    }
    
    checkAlarmState();
    emit configChanged();
}

void ModbusRegister::resetStatistics()
{
    QMutexLocker locker(&mutex);
    updateCount = 0;
    lastUpdateTime = QDateTime();
    emit statisticsReset();
}

void ModbusRegister::checkAlarmState()
{
    if (!valid || !config.isAlarmEnabled) {
        if (alarmState) {
            alarmState = false;
            emit alarmStateChanged(false);
        }
        return;
    }
    
    bool ok;
    double numValue = value.toDouble(&ok);
    if (!ok) return;
    
    bool newAlarmState = (numValue < config.alarmLowLimit || 
                         numValue > config.alarmHighLimit);
    
    if (alarmState != newAlarmState) {
        alarmState = newAlarmState;
        emit alarmStateChanged(alarmState);
    }
}

QVariant ModbusRegister::convertToDataType(const QVariant& value, ModbusTypes::DataType targetType) const
{
    bool ok;
    
    switch (targetType) {
        case ModbusTypes::DataType::BIT:
            return value.toBool();
            
        case ModbusTypes::DataType::BYTE:
            {
                quint8 val = value.toUInt(&ok);
                return ok ? QVariant(val) : QVariant();
            }
            
        case ModbusTypes::DataType::WORD:
            {
                quint16 val = value.toUInt(&ok);
                return ok ? QVariant(val) : QVariant();
            }
            
        case ModbusTypes::DataType::INT:
            {
                qint16 val = value.toInt(&ok);
                return ok ? QVariant(val) : QVariant();
            }
            
        case ModbusTypes::DataType::DWORD:
            {
                quint32 val = value.toUInt(&ok);
                return ok ? QVariant(val) : QVariant();
            }
            
        case ModbusTypes::DataType::DINT:
            {
                qint32 val = value.toInt(&ok);
                return ok ? QVariant(val) : QVariant();
            }
            
        case ModbusTypes::DataType::REAL:
            {
                float val = value.toFloat(&ok);
                return ok ? QVariant(val) : QVariant();
            }
            
        case ModbusTypes::DataType::LREAL:
            {
                double val = value.toDouble(&ok);
                return ok ? QVariant(val) : QVariant();
            }
            
        case ModbusTypes::DataType::STRING:
        case ModbusTypes::DataType::WSTRING:
            return value.toString();
    }
    
    return QVariant();
}

template<typename T>
T ModbusRegister::reorderBytes(T value, ModbusTypes::ByteOrder order) const
{
    switch (order) {
        case ModbusTypes::ByteOrder::AB_CD:  // Big-Endian (default)
            return value;
            
        case ModbusTypes::ByteOrder::CD_AB:  // Little-Endian
            {
                T result = 0;
                char* src = reinterpret_cast<char*>(&value);
                char* dst = reinterpret_cast<char*>(&result);
                for (size_t i = 0; i < sizeof(T); ++i) {
                    dst[i] = src[sizeof(T) - 1 - i];
                }
                return result;
            }
            
        case ModbusTypes::ByteOrder::BA_DC:  // Big-Endian Byte Swap
            {
                T result = 0;
                char* src = reinterpret_cast<char*>(&value);
                char* dst = reinterpret_cast<char*>(&result);
                for (size_t i = 0; i < sizeof(T); i += 2) {
                    dst[i] = src[i + 1];
                    dst[i + 1] = src[i];
                }
                return result;
            }
            
        case ModbusTypes::ByteOrder::DC_BA:  // Little-Endian Byte Swap
            {
                T result = 0;
                char* src = reinterpret_cast<char*>(&value);
                char* dst = reinterpret_cast<char*>(&result);
                for (size_t i = 0; i < sizeof(T); i += 2) {
                    dst[sizeof(T) - i - 2] = src[i + 1];
                    dst[sizeof(T) - i - 1] = src[i];
                }
                return result;
            }
    }
    
    return value;
}

QByteArray ModbusRegister::applyByteOrder(const QByteArray& data) const
{
    if (data.isEmpty()) return data;
    
    QByteArray result = data;
    
    switch (config.dataType) {
        case ModbusTypes::DataType::WORD:
        case ModbusTypes::DataType::INT:
            {
                quint16 value;
                memcpy(&value, data.constData(), sizeof(quint16));
                value = reorderBytes(value, config.byteOrder);
                memcpy(result.data(), &value, sizeof(quint16));
            }
            break;
            
        case ModbusTypes::DataType::DWORD:
        case ModbusTypes::DataType::DINT:
        case ModbusTypes::DataType::REAL:
            {
                quint32 value;
                memcpy(&value, data.constData(), sizeof(quint32));
                value = reorderBytes(value, config.byteOrder);
                memcpy(result.data(), &value, sizeof(quint32));
            }
            break;
            
        case ModbusTypes::DataType::LREAL:
            {
                quint64 value;
                memcpy(&value, data.constData(), sizeof(quint64));
                value = reorderBytes(value, config.byteOrder);
                memcpy(result.data(), &value, sizeof(quint64));
            }
            break;
            
        default:
            break;
    }
    
    return result;
}

QVariant ModbusRegister::convertFromRawData(const QByteArray& data) const
{
    if (data.isEmpty()) return QVariant();
    
    QByteArray orderedData = applyByteOrder(data);
    
    switch (config.dataType) {
        case ModbusTypes::DataType::BIT:
            return orderedData[0] != 0;
            
        case ModbusTypes::DataType::BYTE:
            return static_cast<quint8>(orderedData[0]);
            
        case ModbusTypes::DataType::WORD:
            {
                quint16 value;
                memcpy(&value, orderedData.constData(), sizeof(quint16));
                return value;
            }
            
        case ModbusTypes::DataType::INT:
            {
                qint16 value;
                memcpy(&value, orderedData.constData(), sizeof(qint16));
                return value;
            }
            
        case ModbusTypes::DataType::DWORD:
            {
                quint32 value;
                memcpy(&value, orderedData.constData(), sizeof(quint32));
                return value;
            }
            
        case ModbusTypes::DataType::DINT:
            {
                qint32 value;
                memcpy(&value, orderedData.constData(), sizeof(qint32));
                return value;
            }
            
        case ModbusTypes::DataType::REAL:
            {
                float value;
                memcpy(&value, orderedData.constData(), sizeof(float));
                return value;
            }
            
        case ModbusTypes::DataType::LREAL:
            {
                double value;
                memcpy(&value, orderedData.constData(), sizeof(double));
                return value;
            }
            
        case ModbusTypes::DataType::STRING:
            return QString::fromLatin1(orderedData);
            
        case ModbusTypes::DataType::WSTRING:
            return QString::fromUtf16(reinterpret_cast<const ushort*>(orderedData.constData()));
    }
    
    return QVariant();
}

QByteArray ModbusRegister::convertToRawData(const QVariant& value) const
{
    QByteArray data;
    
    switch (config.dataType) {
        case ModbusTypes::DataType::BIT:
            data.append(value.toBool() ? 1 : 0);
            break;
            
        case ModbusTypes::DataType::BYTE:
            data.append(static_cast<char>(value.toUInt()));
            break;
            
        case ModbusTypes::DataType::WORD:
            {
                quint16 val = value.toUInt();
                data.resize(sizeof(quint16));
                memcpy(data.data(), &val, sizeof(quint16));
            }
            break;
            
        case ModbusTypes::DataType::INT:
            {
                qint16 val = value.toInt();
                data.resize(sizeof(qint16));
                memcpy(data.data(), &val, sizeof(qint16));
            }
            break;
            
        case ModbusTypes::DataType::DWORD:
            {
                quint32 val = value.toUInt();
                data.resize(sizeof(quint32));
                memcpy(data.data(), &val, sizeof(quint32));
            }
            break;
            
        case ModbusTypes::DataType::DINT:
            {
                qint32 val = value.toInt();
                data.resize(sizeof(qint32));
                memcpy(data.data(), &val, sizeof(qint32));
            }
            break;
            
        case ModbusTypes::DataType::REAL:
            {
                float val = value.toFloat();
                data.resize(sizeof(float));
                memcpy(data.data(), &val, sizeof(float));
            }
            break;
            
        case ModbusTypes::DataType::LREAL:
            {
                double val = value.toDouble();
                data.resize(sizeof(double));
                memcpy(data.data(), &val, sizeof(double));
            }
            break;
            
        case ModbusTypes::DataType::STRING:
            data = value.toString().toLatin1();
            break;
            
        case ModbusTypes::DataType::WSTRING:
            {
                QString str = value.toString();
                data.resize(str.length() * sizeof(ushort));
                memcpy(data.data(), str.utf16(), str.length() * sizeof(ushort));
            }
            break;
    }
    
    return applyByteOrder(data);
}

bool ModbusRegister::validateValue(const QVariant& testValue) const
{
    if (!testValue.isValid()) {
        return false;
    }
    
    // Veri tipi kontrolü
    switch (config.dataType) {
        case ModbusTypes::DataType::BIT:
            if (!testValue.canConvert<bool>()) return false;
            break;
            
        case ModbusTypes::DataType::BYTE:
            {
                bool ok;
                quint8 val = testValue.toUInt(&ok);
                if (!ok) return false;
            }
            break;
            
        case ModbusTypes::DataType::WORD:
            {
                bool ok;
                quint16 val = testValue.toUInt(&ok);
                if (!ok) return false;
            }
            break;
            
        case ModbusTypes::DataType::INT:
            {
                bool ok;
                qint16 val = testValue.toInt(&ok);
                if (!ok) return false;
            }
            break;
            
        case ModbusTypes::DataType::DWORD:
            {
                bool ok;
                quint32 val = testValue.toUInt(&ok);
                if (!ok) return false;
            }
            break;
            
        case ModbusTypes::DataType::DINT:
            {
                bool ok;
                qint32 val = testValue.toInt(&ok);
                if (!ok) return false;
            }
            break;
            
        case ModbusTypes::DataType::REAL:
            {
                bool ok;
                float val = testValue.toFloat(&ok);
                if (!ok) return false;
            }
            break;
            
        case ModbusTypes::DataType::LREAL:
            {
                bool ok;
                double val = testValue.toDouble(&ok);
                if (!ok) return false;
            }
            break;
            
        case ModbusTypes::DataType::STRING:
        case ModbusTypes::DataType::WSTRING:
            if (!testValue.canConvert<QString>()) return false;
            break;
    }
    
    // Sayısal değerler için aralık kontrolü
    if (config.dataType != ModbusTypes::DataType::BIT &&
        config.dataType != ModbusTypes::DataType::STRING &&
        config.dataType != ModbusTypes::DataType::WSTRING) {
        
        bool ok;
        double numValue = testValue.toDouble(&ok);
        if (!ok) return false;
        
        if (numValue < config.minValue || numValue > config.maxValue) {
            return false;
        }
    }
    
    return true;
}

bool ModbusRegister::convertValue(const QVariant& input, QVariant& output) const
{
    // Önce ölçeklemeyi uygula
    QVariant scaledValue = scaleValue(input, true);
    if (!scaledValue.isValid()) {
        return false;
    }
    
    // Hedef veri tipine dönüştür
    output = convertToDataType(scaledValue, config.dataType);
    return output.isValid();
}

QVariant ModbusRegister::scaleValue(const QVariant& value, bool inverse) const
{
    if (!value.isValid()) {
        return value;
    }
    
    // String ve bit değerleri için ölçekleme yapma
    if (config.dataType == ModbusTypes::DataType::BIT ||
        config.dataType == ModbusTypes::DataType::STRING ||
        config.dataType == ModbusTypes::DataType::WSTRING) {
        return value;
    }
    
    bool ok;
    double numValue = value.toDouble(&ok);
    if (!ok) {
        return QVariant();
    }
    
    // Ölçeklemeyi uygula
    if (inverse) {
        numValue /= config.scaleFactor;
    } else {
        numValue *= config.scaleFactor;
    }
    
    // Veri tipine göre dönüştür
    switch (config.dataType) {
        case ModbusTypes::DataType::BYTE:
            return static_cast<quint8>(qRound(numValue));
            
        case ModbusTypes::DataType::WORD:
            return static_cast<quint16>(qRound(numValue));
            
        case ModbusTypes::DataType::INT:
            return static_cast<qint16>(qRound(numValue));
            
        case ModbusTypes::DataType::DWORD:
            return static_cast<quint32>(qRound(numValue));
            
        case ModbusTypes::DataType::DINT:
            return static_cast<qint32>(qRound(numValue));
            
        case ModbusTypes::DataType::REAL:
            return static_cast<float>(numValue);
            
        case ModbusTypes::DataType::LREAL:
            return numValue;
            
        default:
            return QVariant();
    }
}

QString ModbusRegister::formatValue(const QVariant& value) const
{
    if (!value.isValid() || !valid) {
        return "---";
    }
    
    QString result;
    
    switch (config.dataType) {
        case ModbusTypes::DataType::BIT:
            result = value.toBool() ? "1" : "0";
            break;
            
        case ModbusTypes::DataType::BYTE:
        case ModbusTypes::DataType::WORD:
        case ModbusTypes::DataType::DWORD:
            result = QString("0x%1").arg(value.toUInt(), 
                    (config.dataType == ModbusTypes::DataType::BYTE ? 2 :
                     config.dataType == ModbusTypes::DataType::WORD ? 4 : 8), 
                    16, QChar('0')).toUpper();
            break;
            
        case ModbusTypes::DataType::INT:
        case ModbusTypes::DataType::DINT:
            result = QString::number(value.toInt());
            break;
            
        case ModbusTypes::DataType::REAL:
            result = QString::number(value.toFloat(), 'f', 3);
            break;
            
        case ModbusTypes::DataType::LREAL:
            result = QString::number(value.toDouble(), 'f', 6);
            break;
            
        case ModbusTypes::DataType::STRING:
        case ModbusTypes::DataType::WSTRING:
            result = value.toString();
            break;
    }
    
    // Ölçeklenmiş değeri göster
    if (config.scaleFactor != 1.0 &&
        config.dataType != ModbusTypes::DataType::BIT &&
        config.dataType != ModbusTypes::DataType::STRING &&
        config.dataType != ModbusTypes::DataType::WSTRING) {
        
        result = QString::number(value.toDouble() * config.scaleFactor, 'f', 3);
    }
    
    // Birimi ekle
    if (!config.unit.isEmpty()) {
        result += " " + config.unit;
    }
    
    return result;
}

void ModbusRegister::timerEvent(QTimerEvent* event)
{
    Q_UNUSED(event);
}