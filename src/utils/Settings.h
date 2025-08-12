#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QVariant>
#include <QSettings>
#include <QMutex>
#include <QMap>
#include <QSet>

class Settings : public QObject {
    Q_OBJECT

public:
    // Ayar kategorileri
    enum class Category {
        GENERAL,        // Genel uygulama ayarları
        CONNECTION,     // Bağlantı ayarları
        DISPLAY,        // Görünüm ayarları
        LOGGING,        // Loglama ayarları
        ADVANCED        // Gelişmiş ayarlar
    };

    // Singleton erişimi
    static Settings& instance();

    // Ayar yönetimi
    void setValue(const QString& key, const QVariant& value, 
                 Category category = Category::GENERAL);
    QVariant value(const QString& key, 
                  const QVariant& defaultValue = QVariant(),
                  Category category = Category::GENERAL) const;
    
    // Kategori yönetimi
    void beginCategory(Category category);
    void endCategory();
    void removeCategory(Category category);
    
    // Ayar dosyası yönetimi
    bool load(const QString& filename = QString());
    bool save(const QString& filename = QString());
    void reset();
    
    // Değişiklik izleme
    bool isDirty() const;
    void setAutoSave(bool enabled);
    
    // Varsayılan değerler
    void setDefaultValue(const QString& key, const QVariant& value,
                        Category category = Category::GENERAL);
    QVariant defaultValue(const QString& key,
                         Category category = Category::GENERAL) const;
    void resetToDefaults();
    
    // Ayar doğrulama
    void setValidator(const QString& key, 
                     std::function<bool(const QVariant&)> validator,
                     Category category = Category::GENERAL);
    bool isValid(const QString& key,
                Category category = Category::GENERAL) const;
    QStringList getInvalidKeys() const;

    // Ayar şifreleme
    void setEncryptionKey(const QByteArray& key);
    void setEncryptedKeys(const QStringList& keys);
    
    // Ayar izleme
    void watchKey(const QString& key, Category category = Category::GENERAL);
    void unwatchKey(const QString& key, Category category = Category::GENERAL);
    
    // Yardımcı fonksiyonlar
    QString getCategoryString(Category category) const;
    QStringList allKeys(Category category = Category::GENERAL) const;
    bool contains(const QString& key, Category category = Category::GENERAL) const;
    void remove(const QString& key, Category category = Category::GENERAL);

public slots:
    // Otomatik kaydetme
    void sync();
    
signals:
    // Değişiklik bildirimleri
    void valueChanged(const QString& key, const QVariant& value, Category category);
    void categoryChanged(Category category);
    void settingsLoaded();
    void settingsSaved();
    void settingsReset();
    void validationFailed(const QString& key, const QVariant& value, Category category);
    void encryptionKeyChanged();

protected:
    // Singleton yapısı için protected constructor
    explicit Settings(QObject* parent = nullptr);
    virtual ~Settings();

private:
    // Ayar depolama
    QSettings* settings;
    QString currentFile;
    bool autoSave;
    bool dirty;
    
    // Şifreleme
    QByteArray encryptionKey;
    QSet<QString> encryptedKeys;
    
    // Varsayılan değerler ve validatörler
    struct SettingInfo {
        QVariant defaultValue;
        std::function<bool(const QVariant&)> validator;
        bool watched;
    };
    QMap<Category, QMap<QString, SettingInfo>> settingInfos;
    
    // Aktif kategori
    Category currentCategory;
    
    // Thread safety
    mutable QMutex mutex;
    
    // Singleton instance
    static Settings* instance_;
    
    // Yardımcı fonksiyonlar
    QString makeKey(const QString& key, Category category) const;
    QVariant encryptValue(const QVariant& value) const;
    QVariant decryptValue(const QVariant& value) const;
    void validateAndNotify(const QString& key, const QVariant& value, Category category);
    void emitChangeSignals(const QString& key, const QVariant& value, Category category);
    void markDirty();
    void validateAllSettings();
    Category categoryFromString(const QString& categoryStr) const;
    void setupDefaultValues();
    
    // Singleton yapısı için private copy constructor ve assignment operator
    Settings(const Settings&) = delete;
    Settings& operator=(const Settings&) = delete;
};

// Global ayar fonksiyonları
#define SET_SETTING(key, value, category) \
    Settings::instance().setValue(key, value, category)

#define GET_SETTING(key, defaultValue, category) \
    Settings::instance().value(key, defaultValue, category)

#define WATCH_SETTING(key, category) \
    Settings::instance().watchKey(key, category)

#define UNWATCH_SETTING(key, category) \
    Settings::instance().unwatchKey(key, category)

#endif // SETTINGS_H