/********************************************************************************
** Form generated from reading UI file 'ConnectionSettingsWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.13
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONNECTIONSETTINGSWIDGET_H
#define UI_CONNECTIONSETTINGSWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ConnectionSettingsWidget
{
public:
    QVBoxLayout *verticalLayout;
    QWidget *mainWidget;
    QVBoxLayout *mainLayout;
    QGroupBox *connectionTypeGroup;
    QHBoxLayout *horizontalLayout;
    QLabel *connectionTypeLabel;
    QComboBox *connectionTypeCombo;
    QStackedWidget *settingsStack;
    QWidget *tcpSettingsWidget;
    QFormLayout *formLayout;
    QLabel *ipAddressLabel;
    QLineEdit *ipAddressEdit;
    QLabel *portLabel;
    QSpinBox *portSpinBox;
    QLabel *networkInterfaceLabel;
    QHBoxLayout *horizontalLayout_2;
    QComboBox *networkInterfaceCombo;
    QPushButton *refreshNetworkButton;
    QWidget *serialSettingsWidget;
    QFormLayout *formLayout_2;
    QLabel *serialPortLabel;
    QHBoxLayout *horizontalLayout_3;
    QComboBox *serialPortCombo;
    QPushButton *refreshSerialButton;
    QLabel *baudRateLabel;
    QComboBox *baudRateCombo;
    QLabel *parityLabel;
    QComboBox *parityCombo;
    QLabel *dataBitsLabel;
    QComboBox *dataBitsCombo;
    QLabel *stopBitsLabel;
    QComboBox *stopBitsCombo;
    QGroupBox *commonSettingsGroup;
    QFormLayout *formLayout_3;
    QLabel *slaveIdLabel;
    QSpinBox *slaveIdSpinBox;
    QLabel *timeoutLabel;
    QSpinBox *timeoutSpinBox;
    QLabel *retryCountLabel;
    QSpinBox *retryCountSpinBox;
    QLabel *byteOrderLabel;
    QComboBox *byteOrderCombo;
    QLabel *readWriteLabel;
    QCheckBox *readWriteCheckBox;
    QLabel *errorRecoveryLabel;
    QComboBox *errorRecoveryCombo;
    QLabel *errorLabel;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *ConnectionSettingsWidget)
    {
        if (ConnectionSettingsWidget->objectName().isEmpty())
            ConnectionSettingsWidget->setObjectName(QString::fromUtf8("ConnectionSettingsWidget"));
        ConnectionSettingsWidget->resize(600, 400);
        verticalLayout = new QVBoxLayout(ConnectionSettingsWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        mainWidget = new QWidget(ConnectionSettingsWidget);
        mainWidget->setObjectName(QString::fromUtf8("mainWidget"));
        mainLayout = new QVBoxLayout(mainWidget);
        mainLayout->setObjectName(QString::fromUtf8("mainLayout"));
        connectionTypeGroup = new QGroupBox(mainWidget);
        connectionTypeGroup->setObjectName(QString::fromUtf8("connectionTypeGroup"));
        horizontalLayout = new QHBoxLayout(connectionTypeGroup);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        connectionTypeLabel = new QLabel(connectionTypeGroup);
        connectionTypeLabel->setObjectName(QString::fromUtf8("connectionTypeLabel"));

        horizontalLayout->addWidget(connectionTypeLabel);

        connectionTypeCombo = new QComboBox(connectionTypeGroup);
        connectionTypeCombo->setObjectName(QString::fromUtf8("connectionTypeCombo"));

        horizontalLayout->addWidget(connectionTypeCombo);


        mainLayout->addWidget(connectionTypeGroup);

        settingsStack = new QStackedWidget(mainWidget);
        settingsStack->setObjectName(QString::fromUtf8("settingsStack"));
        tcpSettingsWidget = new QWidget();
        tcpSettingsWidget->setObjectName(QString::fromUtf8("tcpSettingsWidget"));
        formLayout = new QFormLayout(tcpSettingsWidget);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        ipAddressLabel = new QLabel(tcpSettingsWidget);
        ipAddressLabel->setObjectName(QString::fromUtf8("ipAddressLabel"));

        formLayout->setWidget(0, QFormLayout::LabelRole, ipAddressLabel);

        ipAddressEdit = new QLineEdit(tcpSettingsWidget);
        ipAddressEdit->setObjectName(QString::fromUtf8("ipAddressEdit"));

        formLayout->setWidget(0, QFormLayout::FieldRole, ipAddressEdit);

        portLabel = new QLabel(tcpSettingsWidget);
        portLabel->setObjectName(QString::fromUtf8("portLabel"));

        formLayout->setWidget(1, QFormLayout::LabelRole, portLabel);

        portSpinBox = new QSpinBox(tcpSettingsWidget);
        portSpinBox->setObjectName(QString::fromUtf8("portSpinBox"));
        portSpinBox->setMinimum(1);
        portSpinBox->setMaximum(65535);
        portSpinBox->setValue(502);

        formLayout->setWidget(1, QFormLayout::FieldRole, portSpinBox);

        networkInterfaceLabel = new QLabel(tcpSettingsWidget);
        networkInterfaceLabel->setObjectName(QString::fromUtf8("networkInterfaceLabel"));

        formLayout->setWidget(2, QFormLayout::LabelRole, networkInterfaceLabel);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        networkInterfaceCombo = new QComboBox(tcpSettingsWidget);
        networkInterfaceCombo->setObjectName(QString::fromUtf8("networkInterfaceCombo"));

        horizontalLayout_2->addWidget(networkInterfaceCombo);

        refreshNetworkButton = new QPushButton(tcpSettingsWidget);
        refreshNetworkButton->setObjectName(QString::fromUtf8("refreshNetworkButton"));

        horizontalLayout_2->addWidget(refreshNetworkButton);


        formLayout->setLayout(2, QFormLayout::FieldRole, horizontalLayout_2);

        settingsStack->addWidget(tcpSettingsWidget);
        serialSettingsWidget = new QWidget();
        serialSettingsWidget->setObjectName(QString::fromUtf8("serialSettingsWidget"));
        formLayout_2 = new QFormLayout(serialSettingsWidget);
        formLayout_2->setObjectName(QString::fromUtf8("formLayout_2"));
        serialPortLabel = new QLabel(serialSettingsWidget);
        serialPortLabel->setObjectName(QString::fromUtf8("serialPortLabel"));

        formLayout_2->setWidget(0, QFormLayout::LabelRole, serialPortLabel);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        serialPortCombo = new QComboBox(serialSettingsWidget);
        serialPortCombo->setObjectName(QString::fromUtf8("serialPortCombo"));

        horizontalLayout_3->addWidget(serialPortCombo);

        refreshSerialButton = new QPushButton(serialSettingsWidget);
        refreshSerialButton->setObjectName(QString::fromUtf8("refreshSerialButton"));

        horizontalLayout_3->addWidget(refreshSerialButton);


        formLayout_2->setLayout(0, QFormLayout::FieldRole, horizontalLayout_3);

        baudRateLabel = new QLabel(serialSettingsWidget);
        baudRateLabel->setObjectName(QString::fromUtf8("baudRateLabel"));

        formLayout_2->setWidget(1, QFormLayout::LabelRole, baudRateLabel);

        baudRateCombo = new QComboBox(serialSettingsWidget);
        baudRateCombo->setObjectName(QString::fromUtf8("baudRateCombo"));

        formLayout_2->setWidget(1, QFormLayout::FieldRole, baudRateCombo);

        parityLabel = new QLabel(serialSettingsWidget);
        parityLabel->setObjectName(QString::fromUtf8("parityLabel"));

        formLayout_2->setWidget(2, QFormLayout::LabelRole, parityLabel);

        parityCombo = new QComboBox(serialSettingsWidget);
        parityCombo->setObjectName(QString::fromUtf8("parityCombo"));

        formLayout_2->setWidget(2, QFormLayout::FieldRole, parityCombo);

        dataBitsLabel = new QLabel(serialSettingsWidget);
        dataBitsLabel->setObjectName(QString::fromUtf8("dataBitsLabel"));

        formLayout_2->setWidget(3, QFormLayout::LabelRole, dataBitsLabel);

        dataBitsCombo = new QComboBox(serialSettingsWidget);
        dataBitsCombo->setObjectName(QString::fromUtf8("dataBitsCombo"));

        formLayout_2->setWidget(3, QFormLayout::FieldRole, dataBitsCombo);

        stopBitsLabel = new QLabel(serialSettingsWidget);
        stopBitsLabel->setObjectName(QString::fromUtf8("stopBitsLabel"));

        formLayout_2->setWidget(4, QFormLayout::LabelRole, stopBitsLabel);

        stopBitsCombo = new QComboBox(serialSettingsWidget);
        stopBitsCombo->setObjectName(QString::fromUtf8("stopBitsCombo"));

        formLayout_2->setWidget(4, QFormLayout::FieldRole, stopBitsCombo);

        settingsStack->addWidget(serialSettingsWidget);

        mainLayout->addWidget(settingsStack);

        commonSettingsGroup = new QGroupBox(mainWidget);
        commonSettingsGroup->setObjectName(QString::fromUtf8("commonSettingsGroup"));
        formLayout_3 = new QFormLayout(commonSettingsGroup);
        formLayout_3->setObjectName(QString::fromUtf8("formLayout_3"));
        slaveIdLabel = new QLabel(commonSettingsGroup);
        slaveIdLabel->setObjectName(QString::fromUtf8("slaveIdLabel"));

        formLayout_3->setWidget(0, QFormLayout::LabelRole, slaveIdLabel);

        slaveIdSpinBox = new QSpinBox(commonSettingsGroup);
        slaveIdSpinBox->setObjectName(QString::fromUtf8("slaveIdSpinBox"));
        slaveIdSpinBox->setMinimum(1);
        slaveIdSpinBox->setMaximum(247);

        formLayout_3->setWidget(0, QFormLayout::FieldRole, slaveIdSpinBox);

        timeoutLabel = new QLabel(commonSettingsGroup);
        timeoutLabel->setObjectName(QString::fromUtf8("timeoutLabel"));

        formLayout_3->setWidget(1, QFormLayout::LabelRole, timeoutLabel);

        timeoutSpinBox = new QSpinBox(commonSettingsGroup);
        timeoutSpinBox->setObjectName(QString::fromUtf8("timeoutSpinBox"));
        timeoutSpinBox->setMinimum(100);
        timeoutSpinBox->setMaximum(30000);
        timeoutSpinBox->setValue(1000);

        formLayout_3->setWidget(1, QFormLayout::FieldRole, timeoutSpinBox);

        retryCountLabel = new QLabel(commonSettingsGroup);
        retryCountLabel->setObjectName(QString::fromUtf8("retryCountLabel"));

        formLayout_3->setWidget(2, QFormLayout::LabelRole, retryCountLabel);

        retryCountSpinBox = new QSpinBox(commonSettingsGroup);
        retryCountSpinBox->setObjectName(QString::fromUtf8("retryCountSpinBox"));
        retryCountSpinBox->setMaximum(10);
        retryCountSpinBox->setValue(3);

        formLayout_3->setWidget(2, QFormLayout::FieldRole, retryCountSpinBox);

        byteOrderLabel = new QLabel(commonSettingsGroup);
        byteOrderLabel->setObjectName(QString::fromUtf8("byteOrderLabel"));

        formLayout_3->setWidget(3, QFormLayout::LabelRole, byteOrderLabel);

        byteOrderCombo = new QComboBox(commonSettingsGroup);
        byteOrderCombo->setObjectName(QString::fromUtf8("byteOrderCombo"));

        formLayout_3->setWidget(3, QFormLayout::FieldRole, byteOrderCombo);

        readWriteLabel = new QLabel(commonSettingsGroup);
        readWriteLabel->setObjectName(QString::fromUtf8("readWriteLabel"));

        formLayout_3->setWidget(4, QFormLayout::LabelRole, readWriteLabel);

        readWriteCheckBox = new QCheckBox(commonSettingsGroup);
        readWriteCheckBox->setObjectName(QString::fromUtf8("readWriteCheckBox"));
        readWriteCheckBox->setChecked(true);

        formLayout_3->setWidget(4, QFormLayout::FieldRole, readWriteCheckBox);

        errorRecoveryLabel = new QLabel(commonSettingsGroup);
        errorRecoveryLabel->setObjectName(QString::fromUtf8("errorRecoveryLabel"));

        formLayout_3->setWidget(5, QFormLayout::LabelRole, errorRecoveryLabel);

        errorRecoveryCombo = new QComboBox(commonSettingsGroup);
        errorRecoveryCombo->setObjectName(QString::fromUtf8("errorRecoveryCombo"));

        formLayout_3->setWidget(5, QFormLayout::FieldRole, errorRecoveryCombo);


        mainLayout->addWidget(commonSettingsGroup);

        errorLabel = new QLabel(mainWidget);
        errorLabel->setObjectName(QString::fromUtf8("errorLabel"));
        errorLabel->setStyleSheet(QString::fromUtf8("color: rgb(255, 0, 0);"));

        mainLayout->addWidget(errorLabel);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        mainLayout->addItem(verticalSpacer);


        verticalLayout->addWidget(mainWidget);


        retranslateUi(ConnectionSettingsWidget);

        QMetaObject::connectSlotsByName(ConnectionSettingsWidget);
    } // setupUi

    void retranslateUi(QWidget *ConnectionSettingsWidget)
    {
        ConnectionSettingsWidget->setWindowTitle(QCoreApplication::translate("ConnectionSettingsWidget", "Connection Settings", nullptr));
        connectionTypeGroup->setTitle(QCoreApplication::translate("ConnectionSettingsWidget", "Connection Type", nullptr));
        connectionTypeLabel->setText(QCoreApplication::translate("ConnectionSettingsWidget", "Type:", nullptr));
        ipAddressLabel->setText(QCoreApplication::translate("ConnectionSettingsWidget", "IP Address:", nullptr));
        portLabel->setText(QCoreApplication::translate("ConnectionSettingsWidget", "Port:", nullptr));
        networkInterfaceLabel->setText(QCoreApplication::translate("ConnectionSettingsWidget", "Network Interface:", nullptr));
        refreshNetworkButton->setText(QCoreApplication::translate("ConnectionSettingsWidget", "Refresh", nullptr));
        serialPortLabel->setText(QCoreApplication::translate("ConnectionSettingsWidget", "Serial Port:", nullptr));
        refreshSerialButton->setText(QCoreApplication::translate("ConnectionSettingsWidget", "Refresh", nullptr));
        baudRateLabel->setText(QCoreApplication::translate("ConnectionSettingsWidget", "Baud Rate:", nullptr));
        parityLabel->setText(QCoreApplication::translate("ConnectionSettingsWidget", "Parity:", nullptr));
        dataBitsLabel->setText(QCoreApplication::translate("ConnectionSettingsWidget", "Data Bits:", nullptr));
        stopBitsLabel->setText(QCoreApplication::translate("ConnectionSettingsWidget", "Stop Bits:", nullptr));
        commonSettingsGroup->setTitle(QCoreApplication::translate("ConnectionSettingsWidget", "Common Settings", nullptr));
        slaveIdLabel->setText(QCoreApplication::translate("ConnectionSettingsWidget", "Slave ID:", nullptr));
        timeoutLabel->setText(QCoreApplication::translate("ConnectionSettingsWidget", "Timeout (ms):", nullptr));
        retryCountLabel->setText(QCoreApplication::translate("ConnectionSettingsWidget", "Retry Count:", nullptr));
        byteOrderLabel->setText(QCoreApplication::translate("ConnectionSettingsWidget", "Byte Order:", nullptr));
        readWriteLabel->setText(QCoreApplication::translate("ConnectionSettingsWidget", "Read/Write:", nullptr));
        readWriteCheckBox->setText(QCoreApplication::translate("ConnectionSettingsWidget", "Enable Write Operations", nullptr));
        errorRecoveryLabel->setText(QCoreApplication::translate("ConnectionSettingsWidget", "Error Recovery:", nullptr));
        errorLabel->setText(QCoreApplication::translate("ConnectionSettingsWidget", "Error Message", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ConnectionSettingsWidget: public Ui_ConnectionSettingsWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONNECTIONSETTINGSWIDGET_H
