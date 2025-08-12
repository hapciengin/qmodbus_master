#ifndef REGISTER_SETUP_DIALOG_H
#define REGISTER_SETUP_DIALOG_H

#include "ModbusTypes.h"
#include <QDialog>
#include <QSet>
#include <QMap>
#include <QPushButton>
#include <QDialogButtonBox>

namespace Ui {
class RegisterSetupDialog;
}

class RegisterSetupDialog : public QDialog {
    Q_OBJECT

public:
    explicit RegisterSetupDialog(QWidget* parent = nullptr);
    ~RegisterSetupDialog();

    // Register yapılandırması
    void setRegisterConfig(const ModbusTypes::RegisterConfig& config);
    ModbusTypes::RegisterConfig getRegisterConfig() const;
    
    // Çoklu register düzenleme modu
    void setMultiEditMode(bool enabled);
    bool isMultiEditMode() const;
    
    // Düzenlenebilir alanları ayarla
    void setEditableFields(const QSet<QString>& fields);
    
    // Validasyon durumu
    bool isValid() const;
    QString getValidationError() const;

public slots:
    // Dialog butonları
    void accept() override;
    void reject() override;
    
    // UI güncellemeleri
    void updatePreview();
    bool validateInput();

signals:
    // Değişiklik bildirimleri
    void configChanged();
    void validationStateChanged(bool isValid);
    
    // Hata bildirimleri
    void error(const QString& message);

protected:
    // Event handlers
    void showEvent(QShowEvent* event) override;
    void changeEvent(QEvent* event) override;

private slots:
    // UI event handlers
    void onAddressChanged(int value);
    void onNameChanged(const QString& text);
    void onDescriptionChanged(const QString& text);
    void onDataTypeChanged(int index);
    void onRegisterTypeChanged(int index);
    void onAccessTypeChanged(int index);
    void onByteOrderChanged(int index);
    void onScaleFactorChanged(double value);
    void onUnitChanged(const QString& text);
    void onMinValueChanged(double value);
    void onMaxValueChanged(double value);
    void onAlarmEnabledChanged(bool enabled);
    void onAlarmLowLimitChanged(double value);
    void onAlarmHighLimitChanged(double value);

private:
    Ui::RegisterSetupDialog *ui;
    
    // Mevcut yapılandırma
    ModbusTypes::RegisterConfig currentConfig;
    bool multiEditMode;
    bool isValidConfig;
    QString validationError;
    QSet<QString> editableFields;
    
    // UI eşleştirmeleri
    QMap<ModbusTypes::DataType, int> dataTypeMap;
    QMap<ModbusTypes::RegisterType, int> registerTypeMap;
    QMap<ModbusTypes::ByteOrder, int> byteOrderMap;
    
    // Veri tipi sınırları
    struct DataTypeLimits {
        double minValue;
        double maxValue;
        int precision;
    };
    QMap<ModbusTypes::DataType, DataTypeLimits> dataTypeLimits;
    
    // Yardımcı fonksiyonlar
    void setupUi();
    void setupConnections();
    void setupValidators();
    void setupDataTypeLimits();
    void populateComboBoxes();
    void updateUIState();
    void updateDataTypeUI();
    void updateAlarmUI();
    void updateMinMaxRanges();
    
    // Validasyon yardımcıları
    bool validateAddress() const;
    bool validateName() const;
    bool validateScale() const;
    bool validateMinMax() const;
    bool validateAlarmLimits() const;
    
    // UI güncelleme yardımcıları
    void updateSpinBoxRanges();
    void updateSpinBoxPrecision();
    void updateEditableState();
    void showValidationError(const QString& error);
    void clearValidationError();
    
    // Format yardımcıları
    QString formatAddress(int address) const;
    QString getDataTypeString(ModbusTypes::DataType type) const;
    
    // Yapılandırma dönüşümleri
    ModbusTypes::RegisterConfig uiToConfig() const;
    void configToUi(const ModbusTypes::RegisterConfig& config);
};

#endif // REGISTER_SETUP_DIALOG_H