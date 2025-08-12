#ifndef DEVICE_CONFIG_DIALOG_H
#define DEVICE_CONFIG_DIALOG_H

#include <QDialog>
#include <QMutex>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QComboBox>
#include <QTableWidget>
#include <QPushButton>
#include <QHeaderView>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include "deviceconfig.h"

namespace Ui {
class DeviceConfigDialog;
}

class DeviceConfigDialog : public QDialog {
    Q_OBJECT
public:
    explicit DeviceConfigDialog(DeviceConfig &config, QWidget *parent = nullptr);
    ~DeviceConfigDialog();

signals:
    void configChanged();

private slots:
    void onAddRegister();
    void onRemoveRegister();
    void onOkClicked();
    void onCancelClicked();
    void validateInput();
    void onPlcTypeChanged(int index);
    void onAddressRangeChanged(int value);

private:
    Ui::DeviceConfigDialog *ui;
    DeviceConfig &deviceConfig;
    QMutex mutex;
    bool isValid;

    QLineEdit *ipEdit;
    QSpinBox *portSpin;
    QSpinBox *slaveIdSpin;
    QLineEdit *deviceNameEdit;
    QComboBox *plcTypeCombo;
    QSpinBox *startAddressBox;
    QLabel *addressRangeLabel;

    QSpinBox *startAddressSpin;
    QSpinBox *quantitySpin;
    QComboBox *registerTypeCombo;
    QComboBox *dataTypeCombo;
    QComboBox *byteOrderCombo;

    QTableWidget *registerTable;

    QSpinBox *timeoutSpin;
    QSpinBox *retryCountSpin;

    QPushButton *okButton;
    QPushButton *cancelButton;

    void setupUi();
    void setupAddressRangeUI();
    void updateAddressRange(const QString& plcType);
    void loadConfig();
    void saveConfig();
    void addRegisterRow(int address, const QString &name, const QString &dataType,
                       double scaleFactor, const QString &unit, 
                       const QString &description);
    bool validateRegisterData();
    void updateUIState();
};

#endif // DEVICE_CONFIG_DIALOG_H