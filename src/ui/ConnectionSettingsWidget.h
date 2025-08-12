#ifndef CONNECTION_SETTINGS_WIDGET_H
#define CONNECTION_SETTINGS_WIDGET_H

#include "ModbusTypes.h"
#include <QWidget>
#include <QMap>

namespace Ui {
class ConnectionSettingsWidget;
}

class QLineEdit;
class QSpinBox;
class QComboBox;
class QCheckBox;
class QStackedWidget;
class QSerialPortInfo;

class ConnectionSettingsWidget : public QWidget {
    Q_OBJECT

public:
    explicit ConnectionSettingsWidget(QWidget* parent = nullptr);
    ~ConnectionSettingsWidget();

    // Yapılandırma
    void setConnectionParams(const ModbusTypes::ConnectionParams& params);
    ModbusTypes::ConnectionParams getConnectionParams() const;
    
    // Durum kontrolü
    bool isValid() const;
    QString getValidationError() const;
    
    // UI durumu
    void setEnabled(bool enabled);
    void reset();

public slots:
    // UI güncellemeleri
    void updateSerialPorts();
    void updateNetworkInterfaces();

signals:
    // Değişiklik bildirimleri
    void connectionParamsChanged();
    void validationStateChanged(bool isValid);
    
    // Hata bildirimleri
    void error(const QString& message);

protected:
    // Event handlers
    void showEvent(QShowEvent* event) override;
    void changeEvent(QEvent* event) override;

private slots:
    // UI event handlers
    void onConnectionTypeChanged(int index);
    void onIpAddressChanged(const QString& text);
    void onPortChanged(int value);
    void onSlaveIdChanged(int value);
    void onTimeoutChanged(int value);
    void onRetryCountChanged(int value);
    void onByteOrderChanged(int index);
    void onReadWriteEnabledChanged(bool enabled);
    void onErrorRecoveryModeChanged(int index);
    
    // Serial port ayarları
    void onSerialPortChanged(const QString& port);
    void onBaudRateChanged(int value);
    void onParityChanged(int index);
    void onDataBitsChanged(int value);
    void onStopBitsChanged(int value);
    
    // Yardımcı slot'lar
    void validateSettings();
    void updateUiState();

private:
    Ui::ConnectionSettingsWidget* ui;
    
    // Mevcut yapılandırma
    ModbusTypes::ConnectionParams currentParams;
    bool isValidConfig;
    QString validationError;
    
    // UI bileşenleri
    QMap<ModbusTypes::ConnectionType, int> connectionTypeMap;
    QMap<ModbusTypes::ByteOrder, int> byteOrderMap;
    QMap<ModbusTypes::ErrorRecoveryMode, int> errorRecoveryMap;
    QMap<char, int> parityMap;
    
    // Yardımcı fonksiyonlar
    void setupUi();
    void setupConnections();
    void setupValidators();
    void populateComboBoxes();
    void updateConnectionTypeWidgets();
    bool validateIpAddress(const QString& address) const;
    bool validateSerialPort(const QString& port) const;
    void loadSettings();
    void saveSettings();
    
    // UI güncelleme yardımcıları
    void updateTcpSettings(const ModbusTypes::ConnectionParams& params);
    void updateSerialSettings(const ModbusTypes::ConnectionParams& params);
    void updateCommonSettings(const ModbusTypes::ConnectionParams& params);
    
    // Yapılandırma dönüşümleri
    ModbusTypes::ConnectionParams uiToParams() const;
    void paramsToUi(const ModbusTypes::ConnectionParams& params);
};

#endif // CONNECTION_SETTINGS_WIDGET_H