#include "Settings.h"
#include "Logger.h"
#include "ModbusTypes.h"
#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QCryptographicHash>
#include <QDataStream>
#include <QIODevice>
#include <QDebug>

// Singleton instance
Settings* Settings::instance_ = nullptr;

Settings& Settings::instance()
{
    if (!instance_) {
        instance_ = new Settings();
    }
    return *instance_;
}

Settings::Settings(QObject* parent)
    : QObject(parent)
    , settings(nullptr)
    , autoSave(true)
    , dirty(false)
    , currentCategory(Category::GENERAL)
{
    QString appName = QCoreApplication::applicationName();
    if (appName.isEmpty()) {
        appName = "QModbus";
    }
    
    settings = new QSettings(QSettings::IniFormat, QSettings::UserScope, 
                           "YourCompany", appName);
    
    setupDefaultValues();
}

Settings::~Settings()
{
    if (dirty && autoSave) {
        sync();
    }
    
    if (settings) {
        delete settings;
        settings = nullptr;
    }
}

void Settings::setupDefaultValues()
{
    // Genel ayarlar
    setDefaultValue("Language", "en", Category::GENERAL);
    setDefaultValue("Theme", "Light", Category::GENERAL);
    setDefaultValue("AutoSave", true, Category::GENERAL);
    setDefaultValue("LogLevel", static_cast<int>(Logger::Level::INFO), Category::GENERAL);
    
    // Bağlantı ayarları
    setDefaultValue("DefaultIP", "127.0.0.1", Category::CONNECTION);
    setDefaultValue("DefaultPort", 502, Category::CONNECTION);
    setDefaultValue("DefaultSlaveId", 1, Category::CONNECTION);
    setDefaultValue("DefaultTimeout", 1000, Category::CONNECTION);
    setDefaultValue("DefaultRetryCount", 3, Category::CONNECTION);
    setDefaultValue("DefaultByteOrder", static_cast<int>(ModbusTypes::ByteOrder::AB_CD), Category::CONNECTION);
    
    // Görünüm ayarları
    setDefaultValue("ShowHexAddresses", false, Category::DISPLAY);
    setDefaultValue("ShowScaledValues", true, Category::DISPLAY);
    setDefaultValue("ShowTimestamps", true, Category::DISPLAY);
    setDefaultValue("HighlightAlarms", true, Category::DISPLAY);
    setDefaultValue("FontSize", 9, Category::DISPLAY);
    setDefaultValue("DateTimeFormat", "yyyy-MM-dd HH:mm:ss", Category::DISPLAY);
    
    // Loglama ayarları
    setDefaultValue("LogToFile", true, Category::LOGGING);
    setDefaultValue("LogToConsole", true, Category::LOGGING);
    setDefaultValue("LogFilePath", QDir::homePath() + "/QModbus.log", Category::LOGGING);
    setDefaultValue("MaxLogFileSize", 10 * 1024 * 1024, Category::LOGGING); // 10 MB
    setDefaultValue("MaxLogFiles", 5, Category::LOGGING);
    setDefaultValue("IncludeTimestamp", true, Category::LOGGING);
    setDefaultValue("IncludeLevel", true, Category::LOGGING);
    setDefaultValue("IncludeContext", true, Category::LOGGING);
    
    // Gelişmiş ayarlar
    setDefaultValue("OptimizeRequests", true, Category::ADVANCED);
    setDefaultValue("MaxRequestSize", 125, Category::ADVANCED);
    setDefaultValue("EnableWatchdog", true, Category::ADVANCED);
    setDefaultValue("WatchdogInterval", 5000, Category::ADVANCED);
    setDefaultValue("EnableAutoReconnect", true, Category::ADVANCED);
    setDefaultValue("ReconnectInterval", 5000, Category::ADVANCED);
    setDefaultValue("MaxReconnectAttempts", 5, Category::ADVANCED);
}

void Settings::setValue(const QString& key, const QVariant& value, Category category)
{
    QMutexLocker locker(&mutex);
    
    QString fullKey = makeKey(key, category);
    QVariant oldValue = settings->value(fullKey);
    
    if (oldValue == value) {
        return;
    }
    
    if (settingInfos.contains(category) && settingInfos[category].contains(key)) {
        auto validator = settingInfos[category][key].validator;
        if (validator && !validator(value)) {
            emit validationFailed(key, value, category);
            return;
        }
    }
    
    settings->setValue(fullKey, encryptedKeys.contains(fullKey) ? encryptValue(value) : value);
    markDirty();
    
    emitChangeSignals(key, value, category);
    
    if (autoSave) {
        sync();
    }
}

QVariant Settings::value(const QString& key, const QVariant& defaultValue, Category category) const
{
    QMutexLocker locker(&mutex);
    
    QString fullKey = makeKey(key, category);
    QVariant result;
    
    if (defaultValue.isValid()) {
        result = settings->value(fullKey, defaultValue);
    } 
    else if (settingInfos.contains(category) && settingInfos[category].contains(key)) {
        result = settings->value(fullKey, settingInfos[category][key].defaultValue);
    }
    else {
        result = settings->value(fullKey);
    }
    
    if (encryptedKeys.contains(fullKey)) {
        result = decryptValue(result);
    }
    
    return result;
}

void Settings::beginCategory(Category category)
{
    QMutexLocker locker(&mutex);
    currentCategory = category;
    settings->beginGroup(getCategoryString(category));
}

void Settings::endCategory()
{
    QMutexLocker locker(&mutex);
    settings->endGroup();
    currentCategory = Category::GENERAL;
}

void Settings::removeCategory(Category category)
{
    QMutexLocker locker(&mutex);
    
    settings->beginGroup(getCategoryString(category));
    settings->remove("");
    settings->endGroup();
    
    markDirty();
    emit categoryChanged(category);
}

bool Settings::load(const QString& filename)
{
    QMutexLocker locker(&mutex);
    
    if (settings) {
        delete settings;
        settings = nullptr;
    }
    
    if (!filename.isEmpty()) {
        settings = new QSettings(filename, QSettings::IniFormat);
        currentFile = filename;
    } else {
        QString appName = QCoreApplication::applicationName();
        if (appName.isEmpty()) {
            appName = "QModbus";
        }
        settings = new QSettings(QSettings::IniFormat, QSettings::UserScope, 
                               "YourCompany", appName);
        currentFile = settings->fileName();
    }
    
    validateAllSettings();
    
    dirty = false;
    emit settingsLoaded();
    
    return true;
}

bool Settings::save(const QString& filename)
{
    QMutexLocker locker(&mutex);
    
    if (!filename.isEmpty() && filename != currentFile) {
        QSettings newSettings(filename, QSettings::IniFormat);
        
        for (const QString& key : settings->allKeys()) {
            newSettings.setValue(key, settings->value(key));
        }
        
        newSettings.sync();
        
        if (newSettings.status() != QSettings::NoError) {
            return false;
        }
        
        delete settings;
        settings = new QSettings(filename, QSettings::IniFormat);
        currentFile = filename;
    } else {
        settings->sync();
        
        if (settings->status() != QSettings::NoError) {
            return false;
        }
    }
    
    dirty = false;
    emit settingsSaved();
    
    return true;
}

void Settings::reset()
{
    QMutexLocker locker(&mutex);
    
    settings->clear();
    setupDefaultValues();
    
    markDirty();
    emit settingsReset();
    
    if (autoSave) {
        sync();
    }
}

bool Settings::isDirty() const
{
    QMutexLocker locker(&mutex);
    return dirty;
}

void Settings::setAutoSave(bool enabled)
{
    QMutexLocker locker(&mutex);
    autoSave = enabled;
    
    if (!enabled && dirty) {
        sync();
    }
}

void Settings::setDefaultValue(const QString& key, const QVariant& value, Category category)
{
    QMutexLocker locker(&mutex);
    
    settingInfos[category][key].defaultValue = value;
    
    QString fullKey = makeKey(key, category);
    if (!settings->contains(fullKey)) {
        settings->setValue(fullKey, value);
        markDirty();
    }
}

QVariant Settings::defaultValue(const QString& key, Category category) const
{
    QMutexLocker locker(&mutex);
    
    if (settingInfos.contains(category) && settingInfos[category].contains(key)) {
        return settingInfos[category][key].defaultValue;
    }
    
    return QVariant();
}

void Settings::resetToDefaults()
{
    QMutexLocker locker(&mutex);
    
    for (auto catIt = settingInfos.begin(); catIt != settingInfos.end(); ++catIt) {
        Category category = catIt.key();
        
        for (auto keyIt = catIt.value().begin(); keyIt != catIt.value().end(); ++keyIt) {
            QString key = keyIt.key();
            QVariant defaultVal = keyIt.value().defaultValue;
            
            setValue(key, defaultVal, category);
        }
    }
    
    markDirty();
    emit settingsReset();
    
    if (autoSave) {
        sync();
    }
}

void Settings::setValidator(const QString& key, std::function<bool(const QVariant&)> validator, Category category)
{
    QMutexLocker locker(&mutex);
    
    settingInfos[category][key].validator = validator;
    validateAndNotify(key, value(key, QVariant(), category), category);
}

bool Settings::isValid(const QString& key, Category category) const
{
    QMutexLocker locker(&mutex);
    
    if (!settingInfos.contains(category) || !settingInfos[category].contains(key)) {
        return true;
    }
    
    auto validator = settingInfos[category][key].validator;
    if (!validator) {
        return true;
    }
    
    return validator(value(key, QVariant(), category));
}

QStringList Settings::getInvalidKeys() const
{
    QMutexLocker locker(&mutex);
    QStringList invalidKeys;
    
    for (auto catIt = settingInfos.begin(); catIt != settingInfos.end(); ++catIt) {
        Category category = catIt.key();
        
        for (auto keyIt = catIt.value().begin(); keyIt != catIt.value().end(); ++keyIt) {
            QString key = keyIt.key();
            
            if (!isValid(key, category)) {
                invalidKeys.append(makeKey(key, category));
            }
        }
    }
    
    return invalidKeys;
}

void Settings::setEncryptionKey(const QByteArray& key)
{
    QMutexLocker locker(&mutex);
    
    encryptionKey = key;
    emit encryptionKeyChanged();
    
    for (const QString& key : encryptedKeys) {
        QStringList parts = key.split('/');
        if (parts.size() >= 2) {
            QString settingKey = parts.last();
            Category category = categoryFromString(parts.first());
            
            QVariant val = value(settingKey, QVariant(), category);
            setValue(settingKey, val, category);
        }
    }
}

void Settings::setEncryptedKeys(const QStringList& keys)
{
    QMutexLocker locker(&mutex);
    
    encryptedKeys.clear();
    for (const QString& key : keys) {
        encryptedKeys.insert(key);
    }
    
    for (const QString& key : encryptedKeys) {
        QStringList parts = key.split('/');
        if (parts.size() >= 2) {
            QString settingKey = parts.last();
            Category category = categoryFromString(parts.first());
            
            QVariant val = value(settingKey, QVariant(), category);
            setValue(settingKey, val, category);
        }
    }
}

void Settings::watchKey(const QString& key, Category category)
{
    QMutexLocker locker(&mutex);
    
    if (settingInfos.contains(category) && settingInfos[category].contains(key)) {
        settingInfos[category][key].watched = true;
    } else {
        settingInfos[category][key].watched = true;
        settingInfos[category][key].defaultValue = QVariant();
        settingInfos[category][key].validator = nullptr;
    }
}

void Settings::unwatchKey(const QString& key, Category category)
{
    QMutexLocker locker(&mutex);
    
    if (settingInfos.contains(category) && settingInfos[category].contains(key)) {
        settingInfos[category][key].watched = false;
    }
}

QString Settings::getCategoryString(Category category) const
{
    switch (category) {
        case Category::GENERAL:
            return "General";
        case Category::CONNECTION:
            return "Connection";
        case Category::DISPLAY:
            return "Display";
        case Category::LOGGING:
            return "Logging";
        case Category::ADVANCED:
            return "Advanced";
        default:
            return "Unknown";
    }
}

Settings::Category Settings::categoryFromString(const QString& categoryStr) const
{
    if (categoryStr == "General") return Category::GENERAL;
    if (categoryStr == "Connection") return Category::CONNECTION;
    if (categoryStr == "Display") return Category::DISPLAY;
    if (categoryStr == "Logging") return Category::LOGGING;
    if (categoryStr == "Advanced") return Category::ADVANCED;
    
    return Category::GENERAL;
}

QStringList Settings::allKeys(Category category) const
{
    QMutexLocker locker(&mutex);
    
    if (category == Category::GENERAL) {
        return settings->allKeys();
    }
    
    settings->beginGroup(getCategoryString(category));
    QStringList keys = settings->allKeys();
    settings->endGroup();
    
    return keys;
}

bool Settings::contains(const QString& key, Category category) const
{
    QMutexLocker locker(&mutex);
    
    QString fullKey = makeKey(key, category);
    return settings->contains(fullKey);
}

void Settings::remove(const QString& key, Category category)
{
    QMutexLocker locker(&mutex);
    
    QString fullKey = makeKey(key, category);
    settings->remove(fullKey);
    
    markDirty();
    emit valueChanged(key, QVariant(), category);
    
    if (autoSave) {
        sync();
    }
}

void Settings::sync()
{
    QMutexLocker locker(&mutex);
    
    if (dirty) {
        settings->sync();
        dirty = false;
    }
}

QString Settings::makeKey(const QString& key, Category category) const
{
    return getCategoryString(category) + "/" + key;
}

QVariant Settings::encryptValue(const QVariant& value) const
{
    if (!value.isValid() || encryptionKey.isEmpty()) {
        return value;
    }
    
    QByteArray data;
    {
        QDataStream stream(&data, QIODevice::WriteOnly);
        stream << value;
    }
    
    QByteArray encrypted;
    for (int i = 0; i < data.size(); ++i) {
        encrypted.append(data.at(i) ^ encryptionKey.at(i % encryptionKey.size()));
    }
    
    return encrypted.toBase64();
}

QVariant Settings::decryptValue(const QVariant& value) const
{
    if (!value.isValid() || encryptionKey.isEmpty()) {
        return value;
    }
    
    QByteArray encrypted = QByteArray::fromBase64(value.toByteArray());
    
    QByteArray decrypted;
    for (int i = 0; i < encrypted.size(); ++i) {
        decrypted.append(encrypted.at(i) ^ encryptionKey.at(i % encryptionKey.size()));
    }
    
    QVariant result;
    {
        QDataStream stream(&decrypted, QIODevice::ReadOnly);
        stream >> result;
    }
    
    return result;
}

void Settings::validateAndNotify(const QString& key, const QVariant& value, Category category)
{
    if (!settingInfos.contains(category) || !settingInfos[category].contains(key)) {
        return;
    }
    
    auto validator = settingInfos[category][key].validator;
    if (!validator) {
        return;
    }
    
    if (!validator(value)) {
        emit validationFailed(key, value, category);
    }
}

void Settings::emitChangeSignals(const QString& key, const QVariant& value, Category category)
{
    emit valueChanged(key, value, category);
    emit categoryChanged(category);
    
    if (settingInfos.contains(category) && 
        settingInfos[category].contains(key) && 
        settingInfos[category][key].watched) {
        
        LOG_DEBUG(QString("Watched setting changed: %1/%2 = %3")
                 .arg(getCategoryString(category))
                 .arg(key)
                 .arg(value.toString()), "Settings");
    }
}

void Settings::markDirty()
{
    dirty = true;
}

void Settings::validateAllSettings()
{
    for (auto catIt = settingInfos.begin(); catIt != settingInfos.end(); ++catIt) {
        Category category = catIt.key();
        
        for (auto keyIt = catIt.value().begin(); keyIt != catIt.value().end(); ++keyIt) {
            QString key = keyIt.key();
            auto validator = keyIt.value().validator;
            
            if (validator) {
                QVariant val = value(key, QVariant(), category);
                if (!validator(val)) {
                    emit validationFailed(key, val, category);
                }
            }
        }
    }
}