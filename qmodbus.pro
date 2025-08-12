TARGET = qmodbus
TEMPLATE = app
VERSION = 0.1.0

QT += gui widgets serialport network concurrent charts

CONFIG += debug

SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp \
    src/DeviceConfigDialog.cpp \
    src/core/ModbusDevice.cpp \
    src/core/ModbusRegister.cpp \
    src/core/ModbusConnection.cpp \
    src/ui/ConnectionSettingsWidget.cpp \
    src/ui/RegisterTableModel.cpp \
    src/ui/RegisterSetupDialog.cpp \
    src/utils/Logger.cpp \
    src/utils/Settings.cpp \
    src/utils/DataLogger.cpp \
    3rdparty/libmodbus/src/modbus.c \
    3rdparty/libmodbus/src/modbus-data.c \
    3rdparty/libmodbus/src/modbus-tcp.c \
    src/tcpipsettingswidget.cpp \
    src/ipaddressctrl.cpp \
    src/iplineedit.cpp

HEADERS += \
    src/mainwindow.h \
    src/DeviceConfigDialog.h \
    src/core/ModbusDevice.h \
    src/core/ModbusRegister.h \
    src/core/ModbusConnection.h \
    src/ui/ConnectionSettingsWidget.h \
    src/ui/RegisterTableModel.h \
    src/ui/RegisterSetupDialog.h \
    src/utils/ModbusTypes.h \
    src/utils/Logger.h \
    src/utils/Settings.h \
    src/utils/DataLogger.h \
    3rdparty/libmodbus/src/modbus.h \
    src/imodbus.h \
    src/tcpipsettingswidget.h \
    src/ipaddressctrl.h \
    src/iplineedit.h

FORMS += \
    forms/mainwindow.ui \
    forms/ConnectionSettingsWidget.ui \
    forms/RegisterSetupDialog.ui \
    forms/tcpipsettingswidget.ui \
    forms/ipaddressctrl.ui \
    forms/DeviceConfigDialog.ui

INCLUDEPATH += \
    3rdparty/libmodbus \
    3rdparty/libmodbus/src \
    src \
    src/core \
    src/ui \
    src/utils

win32 {
    DEFINES += WINVER=0x0501
    LIBS += -lws2_32
}

RESOURCES += data/qmodbus.qrc

include(deployment.pri)