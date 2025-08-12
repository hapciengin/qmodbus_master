#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QLabel>
#include <QStringList>
#include <QMap>
#include <QSettings>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTableWidget>
#include "ui_mainwindow.h"
#include "modbus.h"
#include <sys/time.h>
#include "DeviceConfigDialog.h"
#include "deviceconfig.h"
#include "utils/DataLogger.h"


namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void onConnectButtonClicked();
    void onStartPollClicked();
    void onPollRateChanged(int value);
    void onRegisterValueChanged(int row, int column);
    void updateRegisters();
    void onConnectionError();
    void onAddDeviceClicked();
    void onRemoveDeviceClicked();
    void onSaveConfigClicked();
    void onLoadConfigClicked();
    void onDeviceSelectionChanged(int index);
    void onRegisterSetupClicked();

private:
    Ui::MainWindow *ui;
    QMap<int, modbus_t*> modbusContexts;  // PLC bağlantı contexti
    QMap<int, DeviceConfig> deviceConfigs; // PLC yapılandırmaları
    QTimer *pollTimer;    // Veri okuma zamanlayıcısı
    bool isPolling;       // Polling durumu
    int pollRate;         // Polling hızı (ms)
    int currentDeviceId;  // Aktif PLC ID

    void setupPolling();
    void updateRegisterValue(int row, uint16_t value);
    void processReadResults(int deviceId, uint16_t* registers, int count);
    QString getRegisterTypeString(const QString& plcType) const;
    int adjustRegisterAddress(int address, const QString& plcType) const;
    
    // Temel fonksiyonlar
    void setupConnections();
    void setupTable();
    void updateStatus(const QString &status);
    void startPolling();
    void stopPolling();
    
    // PLC iletişim fonksiyonları
    void writeRegister(int deviceId, int addr, uint16_t value);
    void connectDevice(int deviceId);
    void disconnectDevice(int deviceId);
    void setupRegisterTable(const DeviceConfig &config);
    QString formatRegisterValue(uint16_t value, const QString &format, uint16_t nextValue = 0);
    
    // Yapılandırma fonksiyonları
    void saveSettings();
    void loadSettings();
    void updateDeviceList();
    QVariant deviceConfigToVariant(const DeviceConfig &config);
    DeviceConfig variantToDeviceConfig(const QVariant &variant);
    
    // PLC veri işleme fonksiyonları
    double scaleValue(uint16_t rawValue, const RegisterConfig& config);
    void handleCommunicationError(int deviceId, const QString& error);
    bool validateRegisterConfig(const RegisterConfig& config);
    DataLogger* dataLogger;
};  

#endif // MAINWINDOW_H