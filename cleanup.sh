#!/bin/bash

# Gereksiz dosyaları temizle
echo "Gereksiz dosyaları temizleme..."
rm -rf src/serialsettingswidget.* src/rtusettingswidget.* src/asciisettingswidget.*
rm -f forms/serialsettingswidget.ui forms/about.ui forms/BatchProcessor.ui
rm -rf 3rdparty/qextserialport/
rm -f 3rdparty/libmodbus/src/modbus-ascii.*
rm -f 3rdparty/libmodbus/src/modbus-rtu.*
rm -f src/BatchProcessor.*
rm -rf debian/
rm -rf flatpak/
rm -f *.o
rm -f moc_*
rm -f qrc_*
rm -rf build/
rm -f ui_*.h
rm -f qmodbus.desktop qmodbus.nsi qmodbus.rc

# mainwindow.ui dosyasını yeniden oluştur
echo "mainwindow.ui yeniden oluşturuluyor..."
cat > forms/mainwindow.ui << 'EOL'
<?xml version="1.0" encoding="UTF-8"?>
<ui version="4.0">
 <class>MainWindow</class>
 <widget class="QMainWindow" name="MainWindow">
  <property name="geometry">
   <rect>
    <x>0</x>
    <y>0</y>
    <width>800</width>
    <height>600</height>
   </rect>
  </property>
  <property name="windowTitle">
   <string>Modbus Poll</string>
  </property>
  <widget class="QWidget" name="centralwidget">
   <layout class="QVBoxLayout" name="verticalLayout">
    <item>
     <layout class="QHBoxLayout" name="horizontalLayout">
      <item>
       <widget class="QPushButton" name="connectButton">
        <property name="text">
         <string>Connect</string>
        </property>
       </widget>
      </item>
      <item>
       <spacer name="horizontalSpacer">
        <property name="orientation">
         <enum>Qt::Horizontal</enum>
        </property>
        <property name="sizeHint" stdset="0">
         <size>
          <width>40</width>
          <height>20</height>
         </size>
        </property>
       </spacer>
      </item>
     </layout>
    </item>
    <item>
     <widget class="QTableWidget" name="registerTable"/>
    </item>
   </layout>
  </widget>
  <widget class="QMenuBar" name="menubar">
   <property name="geometry">
    <rect>
     <x>0</x>
     <y>0</y>
     <width>800</width>
     <height>22</height>
    </rect>
   </property>
   <widget class="QMenu" name="menuConnection">
    <property name="title">
     <string>Connection</string>
    </property>
    <addaction name="actionConnect"/>
    <addaction name="actionDisconnect"/>
   </widget>
   <addaction name="menuConnection"/>
  </widget>
  <widget class="QStatusBar" name="statusbar"/>
  <action name="actionConnect">
   <property name="text">
    <string>Connect</string>
   </property>
  </action>
  <action name="actionDisconnect">
   <property name="text">
    <string>Disconnect</string>
   </property>
  </action>
 </widget>
 <resources/>
 <connections/>
</ui>
EOL

# mainwindow.h dosyasını yeniden oluştur
echo "mainwindow.h yeniden oluşturuluyor..."
cat > src/mainwindow.h << 'EOL'
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QLabel>
#include "ui_mainwindow.h"
#include "tcpipsettingswidget.h"
#include "modbus.h"

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
    void onConnectionError();
    void updateRegisters();

private:
    Ui::MainWindow *ui;
    modbus_t *ctx;
    QTimer *pollTimer;
    bool isConnected;
    void setupConnections();
    void updateStatus(const QString &status);
};

#endif // MAINWINDOW_H
EOL

# mainwindow.cpp dosyasını yeniden oluştur
echo "mainwindow.cpp yeniden oluşturuluyor..."
cat > src/mainwindow.cpp << 'EOL'
#include "mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    ctx(NULL),
    isConnected(false)
{
    ui->setupUi(this);
    setupConnections();
    
    pollTimer = new QTimer(this);
    connect(pollTimer, &QTimer::timeout, this, &MainWindow::updateRegisters);
}

MainWindow::~MainWindow()
{
    if(ctx) {
        modbus_close(ctx);
        modbus_free(ctx);
    }
    delete ui;
}

void MainWindow::setupConnections()
{
    connect(ui->connectButton, &QPushButton::clicked, this, &MainWindow::onConnectButtonClicked);
}

void MainWindow::onConnectButtonClicked()
{
    // TCP bağlantı kodları buraya gelecek
}

void MainWindow::onConnectionError()
{
    QMessageBox::critical(this, tr("Connection Error"), tr("Failed to connect to the device"));
}

void MainWindow::updateRegisters()
{
    // Register güncelleme kodları buraya gelecek
}

void MainWindow::updateStatus(const QString &status)
{
    ui->statusbar->showMessage(status);
}
EOL

# qmodbus.pro dosyasını güncelle
echo "qmodbus.pro güncelleniyor..."
cat > qmodbus.pro << 'EOL'
TARGET = qmodbus
TEMPLATE = app
VERSION = 0.1.0

QT += gui widgets

SOURCES += src/main.cpp \
    src/mainwindow.cpp \
    3rdparty/libmodbus/src/modbus.c \
    3rdparty/libmodbus/src/modbus-data.c \
    3rdparty/libmodbus/src/modbus-tcp.c \
    src/tcpipsettingswidget.cpp \
    src/ipaddressctrl.cpp \
    src/iplineedit.cpp

HEADERS += src/mainwindow.h \
    3rdparty/libmodbus/src/modbus.h \
    src/imodbus.h \
    src/tcpipsettingswidget.h \
    src/ipaddressctrl.h \
    src/iplineedit.h

INCLUDEPATH += 3rdparty/libmodbus \
               3rdparty/libmodbus/src \
               src

win32 {
    DEFINES += WINVER=0x0501
    LIBS += -lws2_32
}

FORMS += forms/mainwindow.ui \
    forms/tcpipsettingswidget.ui \
    forms/ipaddressctrl.ui

RESOURCES += data/qmodbus.qrc

include(deployment.pri)
EOL

# Temizle ve yeniden derle
echo "Derleme yapılıyor..."
make clean
qmake
make

echo "İşlem tamamlandı!"
