#ifndef MODBUS_DEVICE_H
#define MODBUS_DEVICE_H

#include "ModbusConnection.h"
#include "ModbusRegister.h"
#include "ModbusTypes.h"
#include <QObject>
#include <QMap>
#include <QTimer>
#include <QDateTime>
#include <QMutex>
#include <memory>

// PLC'deki register yapılandırması
struct RegisterConfig {
    int address;          // Register adresi
    QString name;         // Register ismi/açıklaması
    QString dataType;     // Veri tipi (INT16, UINT16, FLOAT32, vs.)
    double scaleFactor;   // Ölçekleme faktörü
    QString unit;         // Birim (°C, Bar, mA, vs.)
    double minValue;      // Minimum değer
    double maxValue;      // Maximum değer

    RegisterConfig() : 
        address(0),
        scaleFactor(1.0),
        minValue(0.0),
        maxValue(65535.0)
    {}
};

// PLC cihaz yapılandırması
struct DeviceConfig {
    QString ip;           // PLC IP adresi
    int port;            // Modbus TCP port
    int slaveId;         // PLC slave ID
    int startAddress;    // Başlangıç register adresi
    int quantity;        // Okunacak register sayısı
    QStringList registerFormats;  // Register format listesi
    bool isActive;       // Bağlantı durumu
    QString deviceName;   // PLC adı
    QString plcType;      // PLC tipi
    QString ipConfig;     // IP yapılandırması
    int timeout;         // Timeout süresi
    int retryCount;      // Tekrar deneme sayısı
    QString registerType; // Register tipi
    QString dataType;     // Veri tipi
    QString byteOrder;    // Byte sıralaması
    int addressOffset;    // PLC tipine göre başlangıç adresi
    QString addressFormat; // Adres format tipi
    QMap<int, RegisterConfig> registers;  // Register konfigürasyonları

    DeviceConfig() :
        port(502),
        slaveId(1),
        startAddress(0),
        quantity(10),
        isActive(false),
        timeout(1000),
        retryCount(3),
        registerType("Holding Register"),
        dataType("INT16"),
        byteOrder("AB CD"),
        addressOffset(40001),
        addressFormat("4xxxx")
    {
        for(int i = 0; i < quantity; i++) {
            registerFormats.append("Dec");
        }
    }
};

class ModbusDevice : public QObject {
    Q_OBJECT

public:
    explicit ModbusDevice(const QString& name, QObject* parent = nullptr);
    virtual ~ModbusDevice();

    // Temel özellikler
    QString getName() const { return deviceName; }
    void setName(const QString& name) { deviceName = name; }
    
    // Bağlantı yönetimi
    bool connectToDevice();
    void disconnectDevice();
    bool isConnected() const;
    bool reconnect();

    // Yapılandırma
    void setConnectionParams(const ModbusTypes::ConnectionParams& params);
    ModbusTypes::ConnectionParams getConnectionParams() const;
    void setPollingInterval(int ms);
    int getPollingInterval() const;

    // Register yönetimi
    bool addRegister(const ModbusTypes::RegisterConfig& config);
    bool removeRegister(int address);
    bool updateRegister(int address, const ModbusTypes::RegisterConfig& config);
    ModbusTypes::RegisterConfig getRegisterConfig(int address) const;
    QList<int> getRegisterAddresses() const;
    QVariant getRegisterValue(int address) const;
    bool setRegisterValue(int address, const QVariant& value);

    // Polling kontrolü
    void startPolling();
    void stopPolling();
    bool isPolling() const;

    // İstatistikler
    int getTotalRequests() const;
    int getSuccessfulRequests() const;
    int getFailedRequests() const;
    double getAverageResponseTime() const;
    void resetStatistics();

    // Hata yönetimi
    QString getLastError() const;
    void clearError();

    // Durum bilgisi
    QDateTime getLastCommunicationTime() const;
    int getRetryCount() const;
    void setRetryCount(int count);

    // Yapılandırma kaydetme/yükleme
    bool saveConfiguration(const QString& filename) const;
    bool loadConfiguration(const QString& filename);

signals:
    void deviceConnected();
    void deviceDisconnected();
    void connectionError(const QString& error);
    void communicationError(const QString& error);
    void registerValueChanged(int address, const QVariant& value);
    void registerAdded(int address);
    void registerRemoved(int address);
    void registerUpdated(int address);
    void pollingStarted();
    void pollingStopped();
    void statisticsUpdated();
    void configurationChanged();

protected:
    virtual void timerEvent(QTimerEvent* event) override;

private slots:
    void onConnectionError(const QString& error);
    void onCommunicationError(const QString& error);
    void onRegisterValueChanged(int address, const QVariant& value);
    void onRequestCompleted(bool success);
    void handlePollingTimeout();
    void handleWatchdogTimeout();

private:
    QString deviceName;
    std::unique_ptr<ModbusConnection> connection;
    ModbusTypes::ConnectionParams connectionParams;
    bool isDeviceConnected;
    mutable QString lastError;

    QMap<int, std::shared_ptr<ModbusRegister>> registers;
    mutable QMutex registerMutex;

    QTimer* pollingTimer;
    QTimer* watchdogTimer;
    bool polling;
    int pollingInterval;
    int watchdogInterval;

    int totalRequests;
    int successfulRequests;
    int failedRequests;
    QVector<double> responseTimes;
    QDateTime lastCommunicationTime;

    void setupTimers();
    void cleanupTimers();
    bool validateRegisterConfig(const ModbusTypes::RegisterConfig& config) const;
    void updateStatistics(bool success, double responseTime);
    void processRegisterUpdates();
    void handleCommunicationTimeout();
    bool optimizeRegisterRequests(QList<ModbusTypes::RegisterConfig>& requests) const;
    void logDebug(const QString& message) const;

    QVariantMap configurationToVariantMap() const;
    bool configurationFromVariantMap(const QVariantMap& map);

    ModbusDevice(const ModbusDevice&) = delete;
    ModbusDevice& operator=(const ModbusDevice&) = delete;
};

#endif // MODBUS_DEVICE_H