#ifndef REGISTER_TABLE_MODEL_H
#define REGISTER_TABLE_MODEL_H

#include "ModbusTypes.h"
#include "ModbusRegister.h"
#include <QAbstractTableModel>
#include <QVector>
#include <QMap>
#include <QColor>
#include <QFont>
#include <QMutex>
#include <memory>

class RegisterTableModel : public QAbstractTableModel {
    Q_OBJECT

public:
    // Format enumeration
    enum class DisplayFormat {
        DEC,        // Decimal
        HEX,        // Hexadecimal
        BIN,        // Binary
        FLOAT,      // Float
        DOUBLE,     // Double
        WORD,       // Word
        DWORD,      // Double Word
        ASCII       // ASCII Text
    };
    Q_ENUM(DisplayFormat)

    // Tablo sütunları
    enum Column {
        ADDRESS,        // Register adresi
        NAME,          // Register adı
        VALUE,         // Güncel değer
        FORMAT,        // Görüntüleme formatı
        TYPE,          // Veri tipi
        ACCESS,        // Erişim tipi (R/W)
        SCALE,         // Ölçekleme faktörü
        UNIT,          // Birim
        MIN_VALUE,     // Minimum değer
        MAX_VALUE,     // Maximum değer
        DESCRIPTION,   // Açıklama
        LAST_UPDATE,   // Son güncelleme zamanı
        STATUS,        // Durum (Valid/Invalid/Alarm)
        COLUMN_COUNT
    };

    explicit RegisterTableModel(QObject* parent = nullptr);
    virtual ~RegisterTableModel();

    // QAbstractTableModel interface
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;

    // Format işlemleri
    static QStringList getAvailableFormats();
    static QString formatToString(DisplayFormat format);
    static DisplayFormat stringToFormat(const QString& str);
    QString formatValue(const QVariant& value, DisplayFormat format) const;
    QString formatValue(const ModbusRegister* reg) const;
    DisplayFormat getRegisterFormat(int address) const;
    bool setRegisterFormat(int address, DisplayFormat format);

    // Register yönetimi
    bool addRegister(const ModbusTypes::RegisterConfig& config);
    bool removeRegister(int address);
    bool updateRegister(int address, const ModbusTypes::RegisterConfig& config);
    ModbusTypes::RegisterConfig getRegisterConfig(int address) const;
    QVector<int> getRegisterAddresses() const;
    
    // Değer işlemleri
    bool setValue(int address, const QVariant& value);
    QVariant getValue(int address) const;
    bool isValid(int address) const;
    
    // Toplu işlemler
    void clear();
    void updateAll();
    void invalidateAll();
    
    // Görünüm ayarları
    void setShowScaledValues(bool show);
    void setShowHexAddresses(bool show);
    void setShowTimestamps(bool show);
    void setHighlightAlarms(bool highlight);
    
    // Veri aktarımı
    bool importFromCsv(const QString& filename);
    bool exportToCsv(const QString& filename) const;

public slots:
    void onRegisterValueChanged(int address, const QVariant& value);
    void onRegisterAlarmStateChanged(int address, bool inAlarm);
    void onRegisterConfigChanged(int address);
    void onFormatChanged(int address, DisplayFormat format);

signals:
    void registerAdded(int address);
    void registerRemoved(int address);
    void registerUpdated(int address);
    void valueChanged(int address, const QVariant& value);
    void alarmStateChanged(int address, bool inAlarm);
    void formatChanged(int address, DisplayFormat format);
    void error(const QString& message);
    void configurationChanged();

private:
    // Register container
    struct RegisterData {
        std::shared_ptr<ModbusRegister> reg;
        DisplayFormat format;
        
        RegisterData() : format(DisplayFormat::DEC) {}
    };
    
    QMap<int, RegisterData> registers;
    QVector<int> sortedAddresses;
    mutable QMutex registerMutex;
    QString lastError;
    
    // Görünüm ayarları
    bool showScaledValues;
    bool showHexAddresses;
    bool showTimestamps;
    bool highlightAlarms;
    
    // Görünüm sabitleri
    struct ViewConstants {
        QColor validColor;
        QColor invalidColor;
        QColor alarmColor;
        QColor readOnlyColor;
        QFont normalFont;
        QFont boldFont;
        QString dateTimeFormat;
    } viewConst;
    
    // Yardımcı fonksiyonlar
    void setupViewConstants();
    void updateSortedAddresses();
    int addressToRow(int address) const;
    int rowToAddress(int row) const;
    QString formatAddress(int address) const;
    QString formatDateTime(const QDateTime& dt) const;
    QString getDataTypeString(ModbusTypes::DataType type) const;
    QString getStatusString(const ModbusRegister* reg) const;
    bool validateRegisterConfig(const ModbusTypes::RegisterConfig& config) const;
    
    // Data handling helpers
    QVariant getDisplayData(const ModbusRegister* reg, int column) const;
    QVariant getEditData(const ModbusRegister* reg, int column) const;
    QVariant getToolTipData(const ModbusRegister* reg, int column) const;
    QVariant getCellBackgroundColor(const ModbusRegister* reg, int column) const;
    QVariant getCellForegroundColor(const ModbusRegister* reg, int column) const;
    QVariant getCellFont(const ModbusRegister* reg, int column) const;
    QVariant getAlignmentForColumn(int column) const;
    bool validateCellEdit(const ModbusRegister* reg, int column, const QVariant& value) const;
    
    // CSV helpers
    QStringList prepareCsvHeader() const;
    QStringList prepareCsvRecord(int address) const;
    bool parseCsvRecord(const QStringList& fields);
    
    // Format helpers
    QString decimalToHex(const QVariant& value) const;
    QString decimalToBinary(const QVariant& value) const;
    QString formatFloat(double value, int precision = 2) const;
    QString formatWord(quint16 value) const;
    QString formatDWord(quint32 value) const;
    QString formatAscii(const QByteArray& data) const;
};

#endif // REGISTER_TABLE_MODEL_H