#ifndef MODBUS_TYPES_H
#define MODBUS_TYPES_H

#include <QString>
#include <QMetaType>

namespace ModbusTypes {

enum class ConnectionType {
    TCP_IP,
    UDP_IP,
    RTU_OVER_TCP,
    RTU_OVER_UDP,
    RTU_SERIAL,
    ASCII_SERIAL
};

enum class DataType {
    BIT,
    BYTE,
    WORD,
    INT,
    DWORD,
    DINT,
    REAL,
    LREAL,
    STRING,
    WSTRING
};

enum class RegisterType {
    DISCRETE_INPUT,    // Function 2
    COIL,             // Function 1, 5, 15
    INPUT_REGISTER,   // Function 4
    HOLDING_REGISTER  // Function 3, 6, 16
};

enum class ByteOrder {
    AB_CD,      // Big-Endian
    CD_AB,      // Little-Endian
    BA_DC,      // Big-Endian Byte Swap
    DC_BA       // Little-Endian Byte Swap
};

enum class ErrorRecoveryMode {
    NONE,
    LINK,
    PROTOCOL,
    LINK_AND_PROTOCOL
};

struct ConnectionParams {
    QString name;           // Bağlantı adı
    QString ip;            // IP adresi
    int port;             // Port numarası
    int slaveId;          // Slave ID
    int timeout;          // Timeout süresi (ms)
    int scanRate;         // Tarama hızı (ms)
    int retryCount;       // Yeniden deneme sayısı
    ConnectionType type;   // Bağlantı tipi
    ByteOrder byteOrder;  // Byte sırası
    bool readWriteEnabled; // Okuma/yazma izni
    ErrorRecoveryMode errorRecovery; // Hata kurtarma modu
    
    // RTU/ASCII için ek parametreler
    QString serialPort;    // COM port
    int baudRate;         // Baud rate
    char parity;          // Parity (N, E, O)
    int dataBits;         // Data bits
    int stopBits;         // Stop bits
    
    ConnectionParams() :
        port(502),
        slaveId(1),
        timeout(1000),
        scanRate(1000),
        retryCount(3),
        type(ConnectionType::TCP_IP),
        byteOrder(ByteOrder::AB_CD),
        readWriteEnabled(true),
        errorRecovery(ErrorRecoveryMode::LINK_AND_PROTOCOL),
        baudRate(9600),
        parity('N'),
        dataBits(8),
        stopBits(1)
    {}
};

struct RegisterConfig {
    int address;           // Register adresi
    QString name;          // Register adı
    QString description;   // Açıklama
    DataType dataType;     // Veri tipi
    RegisterType regType;  // Register tipi
    double scaleFactor;    // Ölçekleme faktörü
    QString unit;          // Birim
    double minValue;       // Minimum değer
    double maxValue;       // Maximum değer
    bool isReadOnly;       // Salt okunur
    bool isAlarmEnabled;   // Alarm aktif
    double alarmHighLimit; // Yüksek alarm limiti
    double alarmLowLimit;  // Düşük alarm limiti
    ByteOrder byteOrder;   // Byte sırası
    
    RegisterConfig() :
        address(0),
        dataType(DataType::WORD),
        regType(RegisterType::HOLDING_REGISTER),
        scaleFactor(1.0),
        minValue(0),
        maxValue(65535),
        isReadOnly(false),
        isAlarmEnabled(false),
        alarmHighLimit(0),
        alarmLowLimit(0),
        byteOrder(ByteOrder::AB_CD)
    {}
};

// Hata kodları
enum class ModbusError {
    NO_ERROR,
    CONNECTION_FAILED,
    TIMEOUT,
    INVALID_RESPONSE,
    INVALID_CRC,
    INVALID_DATA,
    INVALID_ADDRESS,
    INVALID_FUNCTION,
    SLAVE_FAILURE,
    ACKNOWLEDGE,
    SLAVE_BUSY,
    NEGATIVE_ACKNOWLEDGE,
    MEMORY_PARITY_ERROR,
    GATEWAY_PATH_UNAVAILABLE,
    GATEWAY_TARGET_FAILED
};

} // namespace ModbusTypes

// Qt meta-type kayıtları
Q_DECLARE_METATYPE(ModbusTypes::ConnectionType)
Q_DECLARE_METATYPE(ModbusTypes::DataType)
Q_DECLARE_METATYPE(ModbusTypes::RegisterType)
Q_DECLARE_METATYPE(ModbusTypes::ByteOrder)
Q_DECLARE_METATYPE(ModbusTypes::ErrorRecoveryMode)
Q_DECLARE_METATYPE(ModbusTypes::ModbusError)

#endif // MODBUS_TYPES_H