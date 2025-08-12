#ifndef DEVICE_CONFIG_H
#define DEVICE_CONFIG_H

#include <QString>
#include <QStringList>
#include <QMap>

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
    int addressOffset;    // PLC tipine göre başlangıç adresi (YENİ)
    QString addressFormat; // Adres format tipi (YENİ)
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
        addressOffset(40001),  // Varsayılan değer (YENİ)
        addressFormat("4xxxx") // Varsayılan değer (YENİ)
    {
        for(int i = 0; i < quantity; i++) {
            registerFormats.append("Dec");
        }
    }
};

#endif // DEVICE_CONFIG_H