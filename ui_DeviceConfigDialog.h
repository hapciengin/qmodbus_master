/********************************************************************************
** Form generated from reading UI file 'DeviceConfigDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.13
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DEVICECONFIGDIALOG_H
#define UI_DEVICECONFIGDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_DeviceConfigDialog
{
public:
    QVBoxLayout *mainLayout;
    QGroupBox *basicGroup;
    QFormLayout *basicLayout;
    QLabel *ipLabel;
    QLineEdit *ipEdit;
    QLabel *portLabel;
    QSpinBox *portSpin;
    QLabel *slaveIdLabel;
    QSpinBox *slaveIdSpin;
    QLabel *deviceNameLabel;
    QLineEdit *deviceNameEdit;
    QLabel *plcTypeLabel;
    QComboBox *plcTypeCombo;
    QGroupBox *registerSettingsGroup;
    QFormLayout *registerSettingsLayout;
    QLabel *registerTypeLabel;
    QComboBox *registerTypeCombo;
    QLabel *dataTypeLabel;
    QComboBox *dataTypeCombo;
    QLabel *byteOrderLabel;
    QComboBox *byteOrderCombo;
    QGroupBox *communicationGroup;
    QFormLayout *communicationLayout;
    QLabel *timeoutLabel;
    QSpinBox *timeoutSpin;
    QLabel *retryCountLabel;
    QSpinBox *retryCountSpin;
    QTableWidget *registerTable;
    QHBoxLayout *buttonLayout;
    QPushButton *addButton;
    QPushButton *removeButton;
    QSpacerItem *horizontalSpacer;
    QPushButton *okButton;
    QPushButton *cancelButton;

    void setupUi(QDialog *DeviceConfigDialog)
    {
        if (DeviceConfigDialog->objectName().isEmpty())
            DeviceConfigDialog->setObjectName(QString::fromUtf8("DeviceConfigDialog"));
        DeviceConfigDialog->resize(600, 600);
        mainLayout = new QVBoxLayout(DeviceConfigDialog);
        mainLayout->setObjectName(QString::fromUtf8("mainLayout"));
        basicGroup = new QGroupBox(DeviceConfigDialog);
        basicGroup->setObjectName(QString::fromUtf8("basicGroup"));
        basicLayout = new QFormLayout(basicGroup);
        basicLayout->setObjectName(QString::fromUtf8("basicLayout"));
        ipLabel = new QLabel(basicGroup);
        ipLabel->setObjectName(QString::fromUtf8("ipLabel"));

        basicLayout->setWidget(0, QFormLayout::LabelRole, ipLabel);

        ipEdit = new QLineEdit(basicGroup);
        ipEdit->setObjectName(QString::fromUtf8("ipEdit"));

        basicLayout->setWidget(0, QFormLayout::FieldRole, ipEdit);

        portLabel = new QLabel(basicGroup);
        portLabel->setObjectName(QString::fromUtf8("portLabel"));

        basicLayout->setWidget(1, QFormLayout::LabelRole, portLabel);

        portSpin = new QSpinBox(basicGroup);
        portSpin->setObjectName(QString::fromUtf8("portSpin"));

        basicLayout->setWidget(1, QFormLayout::FieldRole, portSpin);

        slaveIdLabel = new QLabel(basicGroup);
        slaveIdLabel->setObjectName(QString::fromUtf8("slaveIdLabel"));

        basicLayout->setWidget(2, QFormLayout::LabelRole, slaveIdLabel);

        slaveIdSpin = new QSpinBox(basicGroup);
        slaveIdSpin->setObjectName(QString::fromUtf8("slaveIdSpin"));

        basicLayout->setWidget(2, QFormLayout::FieldRole, slaveIdSpin);

        deviceNameLabel = new QLabel(basicGroup);
        deviceNameLabel->setObjectName(QString::fromUtf8("deviceNameLabel"));

        basicLayout->setWidget(3, QFormLayout::LabelRole, deviceNameLabel);

        deviceNameEdit = new QLineEdit(basicGroup);
        deviceNameEdit->setObjectName(QString::fromUtf8("deviceNameEdit"));

        basicLayout->setWidget(3, QFormLayout::FieldRole, deviceNameEdit);

        plcTypeLabel = new QLabel(basicGroup);
        plcTypeLabel->setObjectName(QString::fromUtf8("plcTypeLabel"));

        basicLayout->setWidget(4, QFormLayout::LabelRole, plcTypeLabel);

        plcTypeCombo = new QComboBox(basicGroup);
        plcTypeCombo->setObjectName(QString::fromUtf8("plcTypeCombo"));

        basicLayout->setWidget(4, QFormLayout::FieldRole, plcTypeCombo);


        mainLayout->addWidget(basicGroup);

        registerSettingsGroup = new QGroupBox(DeviceConfigDialog);
        registerSettingsGroup->setObjectName(QString::fromUtf8("registerSettingsGroup"));
        registerSettingsLayout = new QFormLayout(registerSettingsGroup);
        registerSettingsLayout->setObjectName(QString::fromUtf8("registerSettingsLayout"));
        registerTypeLabel = new QLabel(registerSettingsGroup);
        registerTypeLabel->setObjectName(QString::fromUtf8("registerTypeLabel"));

        registerSettingsLayout->setWidget(0, QFormLayout::LabelRole, registerTypeLabel);

        registerTypeCombo = new QComboBox(registerSettingsGroup);
        registerTypeCombo->setObjectName(QString::fromUtf8("registerTypeCombo"));

        registerSettingsLayout->setWidget(0, QFormLayout::FieldRole, registerTypeCombo);

        dataTypeLabel = new QLabel(registerSettingsGroup);
        dataTypeLabel->setObjectName(QString::fromUtf8("dataTypeLabel"));

        registerSettingsLayout->setWidget(1, QFormLayout::LabelRole, dataTypeLabel);

        dataTypeCombo = new QComboBox(registerSettingsGroup);
        dataTypeCombo->setObjectName(QString::fromUtf8("dataTypeCombo"));

        registerSettingsLayout->setWidget(1, QFormLayout::FieldRole, dataTypeCombo);

        byteOrderLabel = new QLabel(registerSettingsGroup);
        byteOrderLabel->setObjectName(QString::fromUtf8("byteOrderLabel"));

        registerSettingsLayout->setWidget(2, QFormLayout::LabelRole, byteOrderLabel);

        byteOrderCombo = new QComboBox(registerSettingsGroup);
        byteOrderCombo->setObjectName(QString::fromUtf8("byteOrderCombo"));

        registerSettingsLayout->setWidget(2, QFormLayout::FieldRole, byteOrderCombo);


        mainLayout->addWidget(registerSettingsGroup);

        communicationGroup = new QGroupBox(DeviceConfigDialog);
        communicationGroup->setObjectName(QString::fromUtf8("communicationGroup"));
        communicationLayout = new QFormLayout(communicationGroup);
        communicationLayout->setObjectName(QString::fromUtf8("communicationLayout"));
        timeoutLabel = new QLabel(communicationGroup);
        timeoutLabel->setObjectName(QString::fromUtf8("timeoutLabel"));

        communicationLayout->setWidget(0, QFormLayout::LabelRole, timeoutLabel);

        timeoutSpin = new QSpinBox(communicationGroup);
        timeoutSpin->setObjectName(QString::fromUtf8("timeoutSpin"));

        communicationLayout->setWidget(0, QFormLayout::FieldRole, timeoutSpin);

        retryCountLabel = new QLabel(communicationGroup);
        retryCountLabel->setObjectName(QString::fromUtf8("retryCountLabel"));

        communicationLayout->setWidget(1, QFormLayout::LabelRole, retryCountLabel);

        retryCountSpin = new QSpinBox(communicationGroup);
        retryCountSpin->setObjectName(QString::fromUtf8("retryCountSpin"));

        communicationLayout->setWidget(1, QFormLayout::FieldRole, retryCountSpin);


        mainLayout->addWidget(communicationGroup);

        registerTable = new QTableWidget(DeviceConfigDialog);
        registerTable->setObjectName(QString::fromUtf8("registerTable"));
        registerTable->setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::EditKeyPressed);
        registerTable->setSelectionMode(QAbstractItemView::SingleSelection);
        registerTable->setSelectionBehavior(QAbstractItemView::SelectRows);
        registerTable->setSortingEnabled(false);

        mainLayout->addWidget(registerTable);

        buttonLayout = new QHBoxLayout();
        buttonLayout->setObjectName(QString::fromUtf8("buttonLayout"));
        addButton = new QPushButton(DeviceConfigDialog);
        addButton->setObjectName(QString::fromUtf8("addButton"));

        buttonLayout->addWidget(addButton);

        removeButton = new QPushButton(DeviceConfigDialog);
        removeButton->setObjectName(QString::fromUtf8("removeButton"));

        buttonLayout->addWidget(removeButton);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        buttonLayout->addItem(horizontalSpacer);

        okButton = new QPushButton(DeviceConfigDialog);
        okButton->setObjectName(QString::fromUtf8("okButton"));

        buttonLayout->addWidget(okButton);

        cancelButton = new QPushButton(DeviceConfigDialog);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));

        buttonLayout->addWidget(cancelButton);


        mainLayout->addLayout(buttonLayout);


        retranslateUi(DeviceConfigDialog);

        QMetaObject::connectSlotsByName(DeviceConfigDialog);
    } // setupUi

    void retranslateUi(QDialog *DeviceConfigDialog)
    {
        DeviceConfigDialog->setWindowTitle(QCoreApplication::translate("DeviceConfigDialog", "Device Configuration", nullptr));
        basicGroup->setTitle(QCoreApplication::translate("DeviceConfigDialog", "Basic Settings", nullptr));
        ipLabel->setText(QCoreApplication::translate("DeviceConfigDialog", "IP Address:", nullptr));
        portLabel->setText(QCoreApplication::translate("DeviceConfigDialog", "Port:", nullptr));
        slaveIdLabel->setText(QCoreApplication::translate("DeviceConfigDialog", "Slave ID:", nullptr));
        deviceNameLabel->setText(QCoreApplication::translate("DeviceConfigDialog", "Device Name:", nullptr));
        plcTypeLabel->setText(QCoreApplication::translate("DeviceConfigDialog", "PLC Type:", nullptr));
        registerSettingsGroup->setTitle(QCoreApplication::translate("DeviceConfigDialog", "Register Settings", nullptr));
        registerTypeLabel->setText(QCoreApplication::translate("DeviceConfigDialog", "Register Type:", nullptr));
        dataTypeLabel->setText(QCoreApplication::translate("DeviceConfigDialog", "Data Type:", nullptr));
        byteOrderLabel->setText(QCoreApplication::translate("DeviceConfigDialog", "Byte Order:", nullptr));
        communicationGroup->setTitle(QCoreApplication::translate("DeviceConfigDialog", "Communication Settings", nullptr));
        timeoutLabel->setText(QCoreApplication::translate("DeviceConfigDialog", "Timeout:", nullptr));
        retryCountLabel->setText(QCoreApplication::translate("DeviceConfigDialog", "Retry Count:", nullptr));
        addButton->setText(QCoreApplication::translate("DeviceConfigDialog", "Add Register", nullptr));
        removeButton->setText(QCoreApplication::translate("DeviceConfigDialog", "Remove Register", nullptr));
        okButton->setText(QCoreApplication::translate("DeviceConfigDialog", "OK", nullptr));
        cancelButton->setText(QCoreApplication::translate("DeviceConfigDialog", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DeviceConfigDialog: public Ui_DeviceConfigDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DEVICECONFIGDIALOG_H
