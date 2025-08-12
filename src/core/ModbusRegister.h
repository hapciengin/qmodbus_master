#ifndef MODBUS_REGISTER_H
#define MODBUS_REGISTER_H

#include "ModbusTypes.h"
#include <QObject>
#include <QVariant>
#include <QDateTime>
#include <QMutex>
#include <memory>

class ModbusRegister : public QObject {
    Q_OBJECT

public:
    explicit ModbusRegister(const ModbusTypes::RegisterConfig& config, QObject* parent = nullptr);
    virtual ~ModbusRegister();

    // Temel özellikler
    int getAddress() const { return config.address; }
    QString getName() const { return config.name; }
    QString getDescription() const { return config.description; }
    ModbusTypes::DataType getDataType() const { return config.dataType; }
    ModbusTypes::RegisterType getRegisterType() const { return config.regType; }
    ModbusTypes::ByteOrder getByteOrder() const { return config.byteOrder; }

    // Değer işlemleri
    QVariant getValue() const;
    bool setValue(const QVariant& value);
    QString getFormattedValue() const;
    QByteArray getRawData() const;
    bool setRawData(const QByteArray& data);
    bool validateValue(const QVariant& value) const;
    bool isValid() const { return valid; }
    void invalidate() {
        QMutexLocker locker(&mutex);
        valid = false;
        value = QVariant();
        lastUpdateTime = QDateTime::currentDateTime();
        emit valueChanged(value);
        emit scaledValueChanged(getScaledValue());
        checkAlarmState();
    }

    // Ölçekleme işlemleri
    double getScaleFactor() const { return config.scaleFactor; }
    void setScaleFactor(double factor);
    QString getUnit() const { return config.unit; }
    void setUnit(const QString& unit);
    QVariant getScaledValue() const;
    bool setScaledValue(const QVariant& value);

    // Sınır değerleri
    double getMinValue() const { return config.minValue; }
    void setMinValue(double min);
    double getMaxValue() const { return config.maxValue; }
    void setMaxValue(double max);
    bool isValueInRange(const QVariant& value) const;

    // Alarm yönetimi
    bool isAlarmEnabled() const { return config.isAlarmEnabled; }
    void setAlarmEnabled(bool enabled);
    double getAlarmHighLimit() const { return config.alarmHighLimit; }
    void setAlarmHighLimit(double limit);
    double getAlarmLowLimit() const { return config.alarmLowLimit; }
    void setAlarmLowLimit(double limit);
    bool isInAlarmState() const { return alarmState; }

    // Erişim kontrolü
    bool isReadOnly() const { return config.isReadOnly; }
    void setReadOnly(bool readonly);

    // Yapılandırma
    ModbusTypes::RegisterConfig getConfig() const { return config; }
    void updateConfig(const ModbusTypes::RegisterConfig& newConfig);

    // İstatistikler
    QDateTime getLastUpdateTime() const { return lastUpdateTime; }
    int getUpdateCount() const { return updateCount; }
    void resetStatistics();

signals:
    void valueChanged(const QVariant& newValue);
    void scaledValueChanged(const QVariant& newValue);
    void alarmStateChanged(bool inAlarm);
    void configChanged();
    void statisticsReset();
    void error(const QString& message);

protected:
    virtual void timerEvent(QTimerEvent* event) override;

private:
    // Temel özellikler
    ModbusTypes::RegisterConfig config;
    QVariant value;
    bool valid;
    bool alarmState;

    // İstatistikler
    QDateTime lastUpdateTime;
    int updateCount;

    // Thread safety
    mutable QMutex mutex;

    // Yardımcı fonksiyonlar
    bool convertValue(const QVariant& input, QVariant& output) const;
    QVariant scaleValue(const QVariant& value, bool inverse = false) const;
    void checkAlarmState();
    QString formatValue(const QVariant& value) const;
    
    // Veri tipi dönüşümleri
    QVariant convertToDataType(const QVariant& value, ModbusTypes::DataType targetType) const;
    QVariant convertFromRawData(const QByteArray& data) const;
    QByteArray convertToRawData(const QVariant& value) const;
    
    // Byte sıralama işlemleri
    template<typename T>
    T reorderBytes(T value, ModbusTypes::ByteOrder order) const;
    QByteArray applyByteOrder(const QByteArray& data) const;

private:
    // Kopyalama engelleyiciler
    ModbusRegister(const ModbusRegister&) = delete;
    ModbusRegister& operator=(const ModbusRegister&) = delete;
};

#endif // MODBUS_REGISTER_H