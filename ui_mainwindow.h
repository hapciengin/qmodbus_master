/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.13
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *deviceLayout;
    QComboBox *deviceComboBox;
    QPushButton *addDeviceButton;
    QPushButton *removeDeviceButton;
    QHBoxLayout *controlLayout;
    QPushButton *connectButton;
    QPushButton *startPollButton;
    QLabel *pollRateLabel;
    QSpinBox *pollRateSpinBox;
    QPushButton *setupButton;
    QSpacerItem *horizontalSpacer;
    QTableWidget *registerTable;
    QHBoxLayout *fileLayout;
    QPushButton *saveButton;
    QPushButton *loadButton;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        deviceLayout = new QHBoxLayout();
        deviceLayout->setObjectName(QString::fromUtf8("deviceLayout"));
        deviceComboBox = new QComboBox(centralwidget);
        deviceComboBox->setObjectName(QString::fromUtf8("deviceComboBox"));

        deviceLayout->addWidget(deviceComboBox);

        addDeviceButton = new QPushButton(centralwidget);
        addDeviceButton->setObjectName(QString::fromUtf8("addDeviceButton"));

        deviceLayout->addWidget(addDeviceButton);

        removeDeviceButton = new QPushButton(centralwidget);
        removeDeviceButton->setObjectName(QString::fromUtf8("removeDeviceButton"));

        deviceLayout->addWidget(removeDeviceButton);


        verticalLayout->addLayout(deviceLayout);

        controlLayout = new QHBoxLayout();
        controlLayout->setObjectName(QString::fromUtf8("controlLayout"));
        connectButton = new QPushButton(centralwidget);
        connectButton->setObjectName(QString::fromUtf8("connectButton"));

        controlLayout->addWidget(connectButton);

        startPollButton = new QPushButton(centralwidget);
        startPollButton->setObjectName(QString::fromUtf8("startPollButton"));

        controlLayout->addWidget(startPollButton);

        pollRateLabel = new QLabel(centralwidget);
        pollRateLabel->setObjectName(QString::fromUtf8("pollRateLabel"));

        controlLayout->addWidget(pollRateLabel);

        pollRateSpinBox = new QSpinBox(centralwidget);
        pollRateSpinBox->setObjectName(QString::fromUtf8("pollRateSpinBox"));
        pollRateSpinBox->setMinimum(100);
        pollRateSpinBox->setMaximum(10000);
        pollRateSpinBox->setValue(1000);

        controlLayout->addWidget(pollRateSpinBox);

        setupButton = new QPushButton(centralwidget);
        setupButton->setObjectName(QString::fromUtf8("setupButton"));

        controlLayout->addWidget(setupButton);

        horizontalSpacer = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        controlLayout->addItem(horizontalSpacer);


        verticalLayout->addLayout(controlLayout);

        registerTable = new QTableWidget(centralwidget);
        registerTable->setObjectName(QString::fromUtf8("registerTable"));

        verticalLayout->addWidget(registerTable);

        fileLayout = new QHBoxLayout();
        fileLayout->setObjectName(QString::fromUtf8("fileLayout"));
        saveButton = new QPushButton(centralwidget);
        saveButton->setObjectName(QString::fromUtf8("saveButton"));

        fileLayout->addWidget(saveButton);

        loadButton = new QPushButton(centralwidget);
        loadButton->setObjectName(QString::fromUtf8("loadButton"));

        fileLayout->addWidget(loadButton);


        verticalLayout->addLayout(fileLayout);

        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Modbus Poll", nullptr));
        addDeviceButton->setText(QCoreApplication::translate("MainWindow", "Add Device", nullptr));
        removeDeviceButton->setText(QCoreApplication::translate("MainWindow", "Remove Device", nullptr));
        connectButton->setText(QCoreApplication::translate("MainWindow", "Connect", nullptr));
        startPollButton->setText(QCoreApplication::translate("MainWindow", "Start Polling", nullptr));
        pollRateLabel->setText(QCoreApplication::translate("MainWindow", "Poll Rate:", nullptr));
        pollRateSpinBox->setSuffix(QCoreApplication::translate("MainWindow", " ms", nullptr));
        setupButton->setText(QCoreApplication::translate("MainWindow", "Register Setup", nullptr));
        saveButton->setText(QCoreApplication::translate("MainWindow", "Save Config", nullptr));
        loadButton->setText(QCoreApplication::translate("MainWindow", "Load Config", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
