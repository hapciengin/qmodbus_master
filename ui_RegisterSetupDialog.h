/********************************************************************************
** Form generated from reading UI file 'RegisterSetupDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.13
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REGISTERSETUPDIALOG_H
#define UI_REGISTERSETUPDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_RegisterSetupDialog
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *basicGroup;
    QFormLayout *formLayout;
    QLabel *addressLabel;
    QSpinBox *addressSpinBox;
    QLabel *nameLabel;
    QLineEdit *nameEdit;
    QLabel *descriptionLabel;
    QLineEdit *descriptionEdit;
    QGroupBox *dataTypeGroup;
    QFormLayout *formLayout_2;
    QLabel *dataTypeLabel;
    QComboBox *dataTypeCombo;
    QLabel *registerTypeLabel;
    QComboBox *registerTypeCombo;
    QLabel *accessTypeLabel;
    QComboBox *accessTypeCombo;
    QLabel *byteOrderLabel;
    QComboBox *byteOrderCombo;
    QGroupBox *scalingGroup;
    QFormLayout *formLayout_3;
    QLabel *scaleFactorLabel;
    QDoubleSpinBox *scaleFactorSpinBox;
    QLabel *unitLabel;
    QLineEdit *unitEdit;
    QLabel *minValueLabel;
    QDoubleSpinBox *minValueSpinBox;
    QLabel *maxValueLabel;
    QDoubleSpinBox *maxValueSpinBox;
    QGroupBox *alarmGroup;
    QFormLayout *formLayout_4;
    QLabel *alarmEnabledLabel;
    QCheckBox *alarmEnabledCheck;
    QLabel *alarmLowLimitLabel;
    QDoubleSpinBox *alarmLowLimitSpinBox;
    QLabel *alarmHighLimitLabel;
    QDoubleSpinBox *alarmHighLimitSpinBox;
    QGroupBox *previewGroupBox;
    QVBoxLayout *verticalLayout_2;
    QPlainTextEdit *previewText;
    QLabel *errorLabel;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *RegisterSetupDialog)
    {
        if (RegisterSetupDialog->objectName().isEmpty())
            RegisterSetupDialog->setObjectName(QString::fromUtf8("RegisterSetupDialog"));
        RegisterSetupDialog->resize(600, 500);
        verticalLayout = new QVBoxLayout(RegisterSetupDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        basicGroup = new QGroupBox(RegisterSetupDialog);
        basicGroup->setObjectName(QString::fromUtf8("basicGroup"));
        formLayout = new QFormLayout(basicGroup);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        addressLabel = new QLabel(basicGroup);
        addressLabel->setObjectName(QString::fromUtf8("addressLabel"));

        formLayout->setWidget(0, QFormLayout::LabelRole, addressLabel);

        addressSpinBox = new QSpinBox(basicGroup);
        addressSpinBox->setObjectName(QString::fromUtf8("addressSpinBox"));
        addressSpinBox->setMaximum(65535);

        formLayout->setWidget(0, QFormLayout::FieldRole, addressSpinBox);

        nameLabel = new QLabel(basicGroup);
        nameLabel->setObjectName(QString::fromUtf8("nameLabel"));

        formLayout->setWidget(1, QFormLayout::LabelRole, nameLabel);

        nameEdit = new QLineEdit(basicGroup);
        nameEdit->setObjectName(QString::fromUtf8("nameEdit"));

        formLayout->setWidget(1, QFormLayout::FieldRole, nameEdit);

        descriptionLabel = new QLabel(basicGroup);
        descriptionLabel->setObjectName(QString::fromUtf8("descriptionLabel"));

        formLayout->setWidget(2, QFormLayout::LabelRole, descriptionLabel);

        descriptionEdit = new QLineEdit(basicGroup);
        descriptionEdit->setObjectName(QString::fromUtf8("descriptionEdit"));

        formLayout->setWidget(2, QFormLayout::FieldRole, descriptionEdit);


        verticalLayout->addWidget(basicGroup);

        dataTypeGroup = new QGroupBox(RegisterSetupDialog);
        dataTypeGroup->setObjectName(QString::fromUtf8("dataTypeGroup"));
        formLayout_2 = new QFormLayout(dataTypeGroup);
        formLayout_2->setObjectName(QString::fromUtf8("formLayout_2"));
        dataTypeLabel = new QLabel(dataTypeGroup);
        dataTypeLabel->setObjectName(QString::fromUtf8("dataTypeLabel"));

        formLayout_2->setWidget(0, QFormLayout::LabelRole, dataTypeLabel);

        dataTypeCombo = new QComboBox(dataTypeGroup);
        dataTypeCombo->setObjectName(QString::fromUtf8("dataTypeCombo"));

        formLayout_2->setWidget(0, QFormLayout::FieldRole, dataTypeCombo);

        registerTypeLabel = new QLabel(dataTypeGroup);
        registerTypeLabel->setObjectName(QString::fromUtf8("registerTypeLabel"));

        formLayout_2->setWidget(1, QFormLayout::LabelRole, registerTypeLabel);

        registerTypeCombo = new QComboBox(dataTypeGroup);
        registerTypeCombo->setObjectName(QString::fromUtf8("registerTypeCombo"));

        formLayout_2->setWidget(1, QFormLayout::FieldRole, registerTypeCombo);

        accessTypeLabel = new QLabel(dataTypeGroup);
        accessTypeLabel->setObjectName(QString::fromUtf8("accessTypeLabel"));

        formLayout_2->setWidget(2, QFormLayout::LabelRole, accessTypeLabel);

        accessTypeCombo = new QComboBox(dataTypeGroup);
        accessTypeCombo->setObjectName(QString::fromUtf8("accessTypeCombo"));

        formLayout_2->setWidget(2, QFormLayout::FieldRole, accessTypeCombo);

        byteOrderLabel = new QLabel(dataTypeGroup);
        byteOrderLabel->setObjectName(QString::fromUtf8("byteOrderLabel"));

        formLayout_2->setWidget(3, QFormLayout::LabelRole, byteOrderLabel);

        byteOrderCombo = new QComboBox(dataTypeGroup);
        byteOrderCombo->setObjectName(QString::fromUtf8("byteOrderCombo"));

        formLayout_2->setWidget(3, QFormLayout::FieldRole, byteOrderCombo);


        verticalLayout->addWidget(dataTypeGroup);

        scalingGroup = new QGroupBox(RegisterSetupDialog);
        scalingGroup->setObjectName(QString::fromUtf8("scalingGroup"));
        formLayout_3 = new QFormLayout(scalingGroup);
        formLayout_3->setObjectName(QString::fromUtf8("formLayout_3"));
        scaleFactorLabel = new QLabel(scalingGroup);
        scaleFactorLabel->setObjectName(QString::fromUtf8("scaleFactorLabel"));

        formLayout_3->setWidget(0, QFormLayout::LabelRole, scaleFactorLabel);

        scaleFactorSpinBox = new QDoubleSpinBox(scalingGroup);
        scaleFactorSpinBox->setObjectName(QString::fromUtf8("scaleFactorSpinBox"));
        scaleFactorSpinBox->setDecimals(6);
        scaleFactorSpinBox->setMinimum(-1000000.000000000000000);
        scaleFactorSpinBox->setMaximum(1000000.000000000000000);
        scaleFactorSpinBox->setValue(1.000000000000000);

        formLayout_3->setWidget(0, QFormLayout::FieldRole, scaleFactorSpinBox);

        unitLabel = new QLabel(scalingGroup);
        unitLabel->setObjectName(QString::fromUtf8("unitLabel"));

        formLayout_3->setWidget(1, QFormLayout::LabelRole, unitLabel);

        unitEdit = new QLineEdit(scalingGroup);
        unitEdit->setObjectName(QString::fromUtf8("unitEdit"));

        formLayout_3->setWidget(1, QFormLayout::FieldRole, unitEdit);

        minValueLabel = new QLabel(scalingGroup);
        minValueLabel->setObjectName(QString::fromUtf8("minValueLabel"));

        formLayout_3->setWidget(2, QFormLayout::LabelRole, minValueLabel);

        minValueSpinBox = new QDoubleSpinBox(scalingGroup);
        minValueSpinBox->setObjectName(QString::fromUtf8("minValueSpinBox"));
        minValueSpinBox->setDecimals(6);
        minValueSpinBox->setMinimum(-1000000.000000000000000);
        minValueSpinBox->setMaximum(1000000.000000000000000);

        formLayout_3->setWidget(2, QFormLayout::FieldRole, minValueSpinBox);

        maxValueLabel = new QLabel(scalingGroup);
        maxValueLabel->setObjectName(QString::fromUtf8("maxValueLabel"));

        formLayout_3->setWidget(3, QFormLayout::LabelRole, maxValueLabel);

        maxValueSpinBox = new QDoubleSpinBox(scalingGroup);
        maxValueSpinBox->setObjectName(QString::fromUtf8("maxValueSpinBox"));
        maxValueSpinBox->setDecimals(6);
        maxValueSpinBox->setMinimum(-1000000.000000000000000);
        maxValueSpinBox->setMaximum(1000000.000000000000000);
        maxValueSpinBox->setValue(65535.000000000000000);

        formLayout_3->setWidget(3, QFormLayout::FieldRole, maxValueSpinBox);


        verticalLayout->addWidget(scalingGroup);

        alarmGroup = new QGroupBox(RegisterSetupDialog);
        alarmGroup->setObjectName(QString::fromUtf8("alarmGroup"));
        formLayout_4 = new QFormLayout(alarmGroup);
        formLayout_4->setObjectName(QString::fromUtf8("formLayout_4"));
        alarmEnabledLabel = new QLabel(alarmGroup);
        alarmEnabledLabel->setObjectName(QString::fromUtf8("alarmEnabledLabel"));

        formLayout_4->setWidget(0, QFormLayout::LabelRole, alarmEnabledLabel);

        alarmEnabledCheck = new QCheckBox(alarmGroup);
        alarmEnabledCheck->setObjectName(QString::fromUtf8("alarmEnabledCheck"));

        formLayout_4->setWidget(0, QFormLayout::FieldRole, alarmEnabledCheck);

        alarmLowLimitLabel = new QLabel(alarmGroup);
        alarmLowLimitLabel->setObjectName(QString::fromUtf8("alarmLowLimitLabel"));

        formLayout_4->setWidget(1, QFormLayout::LabelRole, alarmLowLimitLabel);

        alarmLowLimitSpinBox = new QDoubleSpinBox(alarmGroup);
        alarmLowLimitSpinBox->setObjectName(QString::fromUtf8("alarmLowLimitSpinBox"));
        alarmLowLimitSpinBox->setDecimals(6);
        alarmLowLimitSpinBox->setMinimum(-1000000.000000000000000);
        alarmLowLimitSpinBox->setMaximum(1000000.000000000000000);

        formLayout_4->setWidget(1, QFormLayout::FieldRole, alarmLowLimitSpinBox);

        alarmHighLimitLabel = new QLabel(alarmGroup);
        alarmHighLimitLabel->setObjectName(QString::fromUtf8("alarmHighLimitLabel"));

        formLayout_4->setWidget(2, QFormLayout::LabelRole, alarmHighLimitLabel);

        alarmHighLimitSpinBox = new QDoubleSpinBox(alarmGroup);
        alarmHighLimitSpinBox->setObjectName(QString::fromUtf8("alarmHighLimitSpinBox"));
        alarmHighLimitSpinBox->setDecimals(6);
        alarmHighLimitSpinBox->setMinimum(-1000000.000000000000000);
        alarmHighLimitSpinBox->setMaximum(1000000.000000000000000);

        formLayout_4->setWidget(2, QFormLayout::FieldRole, alarmHighLimitSpinBox);


        verticalLayout->addWidget(alarmGroup);

        previewGroupBox = new QGroupBox(RegisterSetupDialog);
        previewGroupBox->setObjectName(QString::fromUtf8("previewGroupBox"));
        verticalLayout_2 = new QVBoxLayout(previewGroupBox);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        previewText = new QPlainTextEdit(previewGroupBox);
        previewText->setObjectName(QString::fromUtf8("previewText"));
        previewText->setReadOnly(true);

        verticalLayout_2->addWidget(previewText);


        verticalLayout->addWidget(previewGroupBox);

        errorLabel = new QLabel(RegisterSetupDialog);
        errorLabel->setObjectName(QString::fromUtf8("errorLabel"));
        errorLabel->setStyleSheet(QString::fromUtf8("color: rgb(255, 0, 0);"));

        verticalLayout->addWidget(errorLabel);

        buttonBox = new QDialogButtonBox(RegisterSetupDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(RegisterSetupDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), RegisterSetupDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), RegisterSetupDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(RegisterSetupDialog);
    } // setupUi

    void retranslateUi(QDialog *RegisterSetupDialog)
    {
        RegisterSetupDialog->setWindowTitle(QCoreApplication::translate("RegisterSetupDialog", "Register Setup", nullptr));
        basicGroup->setTitle(QCoreApplication::translate("RegisterSetupDialog", "Basic Properties", nullptr));
        addressLabel->setText(QCoreApplication::translate("RegisterSetupDialog", "Address:", nullptr));
        nameLabel->setText(QCoreApplication::translate("RegisterSetupDialog", "Name:", nullptr));
        descriptionLabel->setText(QCoreApplication::translate("RegisterSetupDialog", "Description:", nullptr));
        dataTypeGroup->setTitle(QCoreApplication::translate("RegisterSetupDialog", "Data Type", nullptr));
        dataTypeLabel->setText(QCoreApplication::translate("RegisterSetupDialog", "Data Type:", nullptr));
        registerTypeLabel->setText(QCoreApplication::translate("RegisterSetupDialog", "Register Type:", nullptr));
        accessTypeLabel->setText(QCoreApplication::translate("RegisterSetupDialog", "Access Type:", nullptr));
        byteOrderLabel->setText(QCoreApplication::translate("RegisterSetupDialog", "Byte Order:", nullptr));
        scalingGroup->setTitle(QCoreApplication::translate("RegisterSetupDialog", "Scaling and Units", nullptr));
        scaleFactorLabel->setText(QCoreApplication::translate("RegisterSetupDialog", "Scale Factor:", nullptr));
        unitLabel->setText(QCoreApplication::translate("RegisterSetupDialog", "Unit:", nullptr));
        minValueLabel->setText(QCoreApplication::translate("RegisterSetupDialog", "Min Value:", nullptr));
        maxValueLabel->setText(QCoreApplication::translate("RegisterSetupDialog", "Max Value:", nullptr));
        alarmGroup->setTitle(QCoreApplication::translate("RegisterSetupDialog", "Alarm Settings", nullptr));
        alarmEnabledLabel->setText(QCoreApplication::translate("RegisterSetupDialog", "Enable Alarms:", nullptr));
        alarmEnabledCheck->setText(QCoreApplication::translate("RegisterSetupDialog", "Enabled", nullptr));
        alarmLowLimitLabel->setText(QCoreApplication::translate("RegisterSetupDialog", "Low Limit:", nullptr));
        alarmHighLimitLabel->setText(QCoreApplication::translate("RegisterSetupDialog", "High Limit:", nullptr));
        previewGroupBox->setTitle(QCoreApplication::translate("RegisterSetupDialog", "Preview", nullptr));
        errorLabel->setText(QCoreApplication::translate("RegisterSetupDialog", "Error Message", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RegisterSetupDialog: public Ui_RegisterSetupDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REGISTERSETUPDIALOG_H
