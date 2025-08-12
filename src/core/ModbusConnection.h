#ifndef MODBUS_CONNECTION_H
#define MODBUS_CONNECTION_H

#include "ModbusTypes.h"
#include "imodbus.h"
#include <QObject>
#include <QTimer>
#include <QDateTime>
#include <QMutex>
#include <QQueue>
#include <QVector>
#include <modbus.h>

// Modbus sabitleri
#define MODBUS_TCP_DEFAULT_PORT 502
#define MODBUS_UDP_DEFAULT_PORT 502
#define MODBUS_MAX_READ_REGISTERS 125
#define MODBUS_MAX_WRITE_REGISTERS 123
#define MODBUS_MAX_READ_BITS 2000
#define MODBUS_MAX_WRITE_BITS 1968

struct ModbusRequest {
    ModbusRequest() : address(0), quantity(0), isWrite(false) {}
    
    int address;
    int quantity;
    ModbusTypes::RegisterType type;
    QByteArray writeData;
    bool isWrite;
    QDateTime timestamp;
};

class ModbusConnection : public QObject, public IModbus {
    Q_OBJECT

public:
    explicit ModbusConnection(QObject* parent = nullptr);
    virtual ~ModbusConnection();

    // IModbus interface implementation
    virtual modbus_t* modbus() override { return ctx; }
    virtual int setupModbusPort() override;

    // Bağlantı yönetimi
    bool connectDevice(const ModbusTypes::ConnectionParams& params);
    void disconnectDevice();
    bool isConnected() const { return m_isConnected; }
    bool reconnect();

    // Okuma işlemleri
    bool readCoils(int addr, int nb, uint8_t* dest);
    bool readDiscreteInputs(int addr, int nb, uint8_t* dest);
    bool readHoldingRegisters(int addr, int nb, uint16_t* dest);
    bool readInputRegisters(int addr, int nb, uint16_t* dest);

    // Yazma işlemleri
    bool writeCoil(int addr, int status);
    bool writeRegister(int addr, int value);
    bool writeMultipleCoils(int addr, int nb, const uint8_t* src);
    bool writeMultipleRegisters(int addr, int nb, const uint16_t* src);

    // Gelişmiş işlemler
    bool readWriteMultipleRegisters(int read_addr, int read_nb, uint16_t* dest,
                                  int write_addr, int write_nb, const uint16_t* src);
    bool maskWriteRegister(int addr, uint16_t and_mask, uint16_t or_mask);

    // Yapılandırma
    void setResponseTimeout(uint32_t ms);
    void setByteTimeout(uint32_t ms);
    void setDebugMode(bool enabled);
    void setErrorRecoveryMode(ModbusTypes::ErrorRecoveryMode mode);

    // Durum bilgisi
    ModbusTypes::ConnectionParams getConnectionParams() const { return params; }
    QString getLastError() const { return lastError; }
    int getRetryCount() const { return params.retryCount; }
    QDateTime getLastCommunicationTime() const { return lastCommunicationTime; }

    // İstatistikler
    int getTotalRequests() const { return totalRequests; }
    int getSuccessfulRequests() const { return successfulRequests; }
    int getFailedRequests() const { return failedRequests; }
    double getAverageResponseTime() const;
    void resetStatistics();

signals:
    void deviceConnected();
    void deviceDisconnected();
    void connectionError(const QString& error);
    void communicationError(const QString& error);
    void requestCompleted(bool success);
    void debugMessage(const QString& message);
    void statisticsUpdated();

protected:
    virtual void timerEvent(QTimerEvent* event) override;

private slots:
    void processQueue();
    void handleTimeout();
    void checkConnection();

private:
    // Temel özellikler
    modbus_t* ctx;
    ModbusTypes::ConnectionParams params;
    bool m_isConnected;
    QString lastError;

    // Timer ayarları ve nesneleri
    QTimer* queueTimer;
    QTimer* watchdogTimer;
    const int queueProcessingInterval;
    const int watchdogInterval;

    // İstatistikler
    int totalRequests;
    int successfulRequests;
    int failedRequests;
    QVector<double> responseTimes;
    QDateTime lastCommunicationTime;

    // Request kuyruğu yönetimi
    QQueue<ModbusRequest> requestQueue;
    QMutex queueMutex;

    // Bağlantı yönetimi
    bool setupConnection();
    void cleanupConnection();
    bool validateConnection() const;

    // Request işleme
    bool executeRequest(const ModbusRequest& request);
    void enqueueRequest(const ModbusRequest& request);
    bool validateRequest(const ModbusRequest& request) const;
    bool processResponse(int result, const ModbusRequest& request);

    // Yardımcı fonksiyonlar
    bool validateAddress(int addr, int quantity, ModbusTypes::RegisterType type) const;
    QString formatModbusError(int errorCode) const;
    void updateStatistics(bool success, double responseTime);
    void logDebug(const QString& message) const;

    // Bağlantı tipleri için özel kurulumlar
    bool setupTcpConnection();
    bool setupUdpConnection();
    bool setupRtuOverTcpConnection();
    bool setupRtuOverUdpConnection();
    bool setupSerialConnection();

    // Özel ayarlar
    bool configureSerialPort();
    bool configureTimeouts();
    bool configureSlaveId();

private:
    // Kopyalama engelleyiciler
    ModbusConnection(const ModbusConnection&) = delete;
    ModbusConnection& operator=(const ModbusConnection&) = delete;
};

#endif // MODBUS_CONNECTION_H