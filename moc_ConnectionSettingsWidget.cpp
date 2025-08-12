/****************************************************************************
** Meta object code from reading C++ file 'ConnectionSettingsWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.13)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "src/ui/ConnectionSettingsWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ConnectionSettingsWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.13. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ConnectionSettingsWidget_t {
    QByteArrayData data[30];
    char stringdata0[471];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ConnectionSettingsWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ConnectionSettingsWidget_t qt_meta_stringdata_ConnectionSettingsWidget = {
    {
QT_MOC_LITERAL(0, 0, 24), // "ConnectionSettingsWidget"
QT_MOC_LITERAL(1, 25, 23), // "connectionParamsChanged"
QT_MOC_LITERAL(2, 49, 0), // ""
QT_MOC_LITERAL(3, 50, 22), // "validationStateChanged"
QT_MOC_LITERAL(4, 73, 7), // "isValid"
QT_MOC_LITERAL(5, 81, 5), // "error"
QT_MOC_LITERAL(6, 87, 7), // "message"
QT_MOC_LITERAL(7, 95, 17), // "updateSerialPorts"
QT_MOC_LITERAL(8, 113, 23), // "updateNetworkInterfaces"
QT_MOC_LITERAL(9, 137, 23), // "onConnectionTypeChanged"
QT_MOC_LITERAL(10, 161, 5), // "index"
QT_MOC_LITERAL(11, 167, 18), // "onIpAddressChanged"
QT_MOC_LITERAL(12, 186, 4), // "text"
QT_MOC_LITERAL(13, 191, 13), // "onPortChanged"
QT_MOC_LITERAL(14, 205, 5), // "value"
QT_MOC_LITERAL(15, 211, 16), // "onSlaveIdChanged"
QT_MOC_LITERAL(16, 228, 16), // "onTimeoutChanged"
QT_MOC_LITERAL(17, 245, 19), // "onRetryCountChanged"
QT_MOC_LITERAL(18, 265, 18), // "onByteOrderChanged"
QT_MOC_LITERAL(19, 284, 25), // "onReadWriteEnabledChanged"
QT_MOC_LITERAL(20, 310, 7), // "enabled"
QT_MOC_LITERAL(21, 318, 26), // "onErrorRecoveryModeChanged"
QT_MOC_LITERAL(22, 345, 19), // "onSerialPortChanged"
QT_MOC_LITERAL(23, 365, 4), // "port"
QT_MOC_LITERAL(24, 370, 17), // "onBaudRateChanged"
QT_MOC_LITERAL(25, 388, 15), // "onParityChanged"
QT_MOC_LITERAL(26, 404, 17), // "onDataBitsChanged"
QT_MOC_LITERAL(27, 422, 17), // "onStopBitsChanged"
QT_MOC_LITERAL(28, 440, 16), // "validateSettings"
QT_MOC_LITERAL(29, 457, 13) // "updateUiState"

    },
    "ConnectionSettingsWidget\0"
    "connectionParamsChanged\0\0"
    "validationStateChanged\0isValid\0error\0"
    "message\0updateSerialPorts\0"
    "updateNetworkInterfaces\0onConnectionTypeChanged\0"
    "index\0onIpAddressChanged\0text\0"
    "onPortChanged\0value\0onSlaveIdChanged\0"
    "onTimeoutChanged\0onRetryCountChanged\0"
    "onByteOrderChanged\0onReadWriteEnabledChanged\0"
    "enabled\0onErrorRecoveryModeChanged\0"
    "onSerialPortChanged\0port\0onBaudRateChanged\0"
    "onParityChanged\0onDataBitsChanged\0"
    "onStopBitsChanged\0validateSettings\0"
    "updateUiState"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ConnectionSettingsWidget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      21,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,  119,    2, 0x06 /* Public */,
       3,    1,  120,    2, 0x06 /* Public */,
       5,    1,  123,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    0,  126,    2, 0x0a /* Public */,
       8,    0,  127,    2, 0x0a /* Public */,
       9,    1,  128,    2, 0x08 /* Private */,
      11,    1,  131,    2, 0x08 /* Private */,
      13,    1,  134,    2, 0x08 /* Private */,
      15,    1,  137,    2, 0x08 /* Private */,
      16,    1,  140,    2, 0x08 /* Private */,
      17,    1,  143,    2, 0x08 /* Private */,
      18,    1,  146,    2, 0x08 /* Private */,
      19,    1,  149,    2, 0x08 /* Private */,
      21,    1,  152,    2, 0x08 /* Private */,
      22,    1,  155,    2, 0x08 /* Private */,
      24,    1,  158,    2, 0x08 /* Private */,
      25,    1,  161,    2, 0x08 /* Private */,
      26,    1,  164,    2, 0x08 /* Private */,
      27,    1,  167,    2, 0x08 /* Private */,
      28,    0,  170,    2, 0x08 /* Private */,
      29,    0,  171,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    4,
    QMetaType::Void, QMetaType::QString,    6,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void, QMetaType::QString,   12,
    QMetaType::Void, QMetaType::Int,   14,
    QMetaType::Void, QMetaType::Int,   14,
    QMetaType::Void, QMetaType::Int,   14,
    QMetaType::Void, QMetaType::Int,   14,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void, QMetaType::Bool,   20,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void, QMetaType::QString,   23,
    QMetaType::Void, QMetaType::Int,   14,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void, QMetaType::Int,   14,
    QMetaType::Void, QMetaType::Int,   14,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void ConnectionSettingsWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ConnectionSettingsWidget *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->connectionParamsChanged(); break;
        case 1: _t->validationStateChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->error((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->updateSerialPorts(); break;
        case 4: _t->updateNetworkInterfaces(); break;
        case 5: _t->onConnectionTypeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->onIpAddressChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 7: _t->onPortChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->onSlaveIdChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->onTimeoutChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->onRetryCountChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->onByteOrderChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: _t->onReadWriteEnabledChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 13: _t->onErrorRecoveryModeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 14: _t->onSerialPortChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 15: _t->onBaudRateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 16: _t->onParityChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 17: _t->onDataBitsChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 18: _t->onStopBitsChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 19: _t->validateSettings(); break;
        case 20: _t->updateUiState(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ConnectionSettingsWidget::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ConnectionSettingsWidget::connectionParamsChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ConnectionSettingsWidget::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ConnectionSettingsWidget::validationStateChanged)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (ConnectionSettingsWidget::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ConnectionSettingsWidget::error)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ConnectionSettingsWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_ConnectionSettingsWidget.data,
    qt_meta_data_ConnectionSettingsWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ConnectionSettingsWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ConnectionSettingsWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ConnectionSettingsWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int ConnectionSettingsWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 21)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 21;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 21)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 21;
    }
    return _id;
}

// SIGNAL 0
void ConnectionSettingsWidget::connectionParamsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void ConnectionSettingsWidget::validationStateChanged(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void ConnectionSettingsWidget::error(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
