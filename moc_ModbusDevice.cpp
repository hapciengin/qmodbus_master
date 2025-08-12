/****************************************************************************
** Meta object code from reading C++ file 'ModbusDevice.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.13)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "src/core/ModbusDevice.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ModbusDevice.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.13. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ModbusDevice_t {
    QByteArrayData data[24];
    char stringdata0[372];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ModbusDevice_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ModbusDevice_t qt_meta_stringdata_ModbusDevice = {
    {
QT_MOC_LITERAL(0, 0, 12), // "ModbusDevice"
QT_MOC_LITERAL(1, 13, 15), // "deviceConnected"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 18), // "deviceDisconnected"
QT_MOC_LITERAL(4, 49, 15), // "connectionError"
QT_MOC_LITERAL(5, 65, 5), // "error"
QT_MOC_LITERAL(6, 71, 18), // "communicationError"
QT_MOC_LITERAL(7, 90, 20), // "registerValueChanged"
QT_MOC_LITERAL(8, 111, 7), // "address"
QT_MOC_LITERAL(9, 119, 5), // "value"
QT_MOC_LITERAL(10, 125, 13), // "registerAdded"
QT_MOC_LITERAL(11, 139, 15), // "registerRemoved"
QT_MOC_LITERAL(12, 155, 15), // "registerUpdated"
QT_MOC_LITERAL(13, 171, 14), // "pollingStarted"
QT_MOC_LITERAL(14, 186, 14), // "pollingStopped"
QT_MOC_LITERAL(15, 201, 17), // "statisticsUpdated"
QT_MOC_LITERAL(16, 219, 20), // "configurationChanged"
QT_MOC_LITERAL(17, 240, 17), // "onConnectionError"
QT_MOC_LITERAL(18, 258, 20), // "onCommunicationError"
QT_MOC_LITERAL(19, 279, 22), // "onRegisterValueChanged"
QT_MOC_LITERAL(20, 302, 18), // "onRequestCompleted"
QT_MOC_LITERAL(21, 321, 7), // "success"
QT_MOC_LITERAL(22, 329, 20), // "handlePollingTimeout"
QT_MOC_LITERAL(23, 350, 21) // "handleWatchdogTimeout"

    },
    "ModbusDevice\0deviceConnected\0\0"
    "deviceDisconnected\0connectionError\0"
    "error\0communicationError\0registerValueChanged\0"
    "address\0value\0registerAdded\0registerRemoved\0"
    "registerUpdated\0pollingStarted\0"
    "pollingStopped\0statisticsUpdated\0"
    "configurationChanged\0onConnectionError\0"
    "onCommunicationError\0onRegisterValueChanged\0"
    "onRequestCompleted\0success\0"
    "handlePollingTimeout\0handleWatchdogTimeout"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ModbusDevice[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      18,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      12,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,  104,    2, 0x06 /* Public */,
       3,    0,  105,    2, 0x06 /* Public */,
       4,    1,  106,    2, 0x06 /* Public */,
       6,    1,  109,    2, 0x06 /* Public */,
       7,    2,  112,    2, 0x06 /* Public */,
      10,    1,  117,    2, 0x06 /* Public */,
      11,    1,  120,    2, 0x06 /* Public */,
      12,    1,  123,    2, 0x06 /* Public */,
      13,    0,  126,    2, 0x06 /* Public */,
      14,    0,  127,    2, 0x06 /* Public */,
      15,    0,  128,    2, 0x06 /* Public */,
      16,    0,  129,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      17,    1,  130,    2, 0x08 /* Private */,
      18,    1,  133,    2, 0x08 /* Private */,
      19,    2,  136,    2, 0x08 /* Private */,
      20,    1,  141,    2, 0x08 /* Private */,
      22,    0,  144,    2, 0x08 /* Private */,
      23,    0,  145,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::Int, QMetaType::QVariant,    8,    9,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::Int, QMetaType::QVariant,    8,    9,
    QMetaType::Void, QMetaType::Bool,   21,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void ModbusDevice::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ModbusDevice *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->deviceConnected(); break;
        case 1: _t->deviceDisconnected(); break;
        case 2: _t->connectionError((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->communicationError((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: _t->registerValueChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2]))); break;
        case 5: _t->registerAdded((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->registerRemoved((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->registerUpdated((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->pollingStarted(); break;
        case 9: _t->pollingStopped(); break;
        case 10: _t->statisticsUpdated(); break;
        case 11: _t->configurationChanged(); break;
        case 12: _t->onConnectionError((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 13: _t->onCommunicationError((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 14: _t->onRegisterValueChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2]))); break;
        case 15: _t->onRequestCompleted((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 16: _t->handlePollingTimeout(); break;
        case 17: _t->handleWatchdogTimeout(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ModbusDevice::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ModbusDevice::deviceConnected)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ModbusDevice::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ModbusDevice::deviceDisconnected)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (ModbusDevice::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ModbusDevice::connectionError)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (ModbusDevice::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ModbusDevice::communicationError)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (ModbusDevice::*)(int , const QVariant & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ModbusDevice::registerValueChanged)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (ModbusDevice::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ModbusDevice::registerAdded)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (ModbusDevice::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ModbusDevice::registerRemoved)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (ModbusDevice::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ModbusDevice::registerUpdated)) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (ModbusDevice::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ModbusDevice::pollingStarted)) {
                *result = 8;
                return;
            }
        }
        {
            using _t = void (ModbusDevice::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ModbusDevice::pollingStopped)) {
                *result = 9;
                return;
            }
        }
        {
            using _t = void (ModbusDevice::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ModbusDevice::statisticsUpdated)) {
                *result = 10;
                return;
            }
        }
        {
            using _t = void (ModbusDevice::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ModbusDevice::configurationChanged)) {
                *result = 11;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ModbusDevice::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_ModbusDevice.data,
    qt_meta_data_ModbusDevice,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ModbusDevice::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ModbusDevice::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ModbusDevice.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int ModbusDevice::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 18)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 18;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 18)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 18;
    }
    return _id;
}

// SIGNAL 0
void ModbusDevice::deviceConnected()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void ModbusDevice::deviceDisconnected()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void ModbusDevice::connectionError(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void ModbusDevice::communicationError(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void ModbusDevice::registerValueChanged(int _t1, const QVariant & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void ModbusDevice::registerAdded(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void ModbusDevice::registerRemoved(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void ModbusDevice::registerUpdated(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void ModbusDevice::pollingStarted()
{
    QMetaObject::activate(this, &staticMetaObject, 8, nullptr);
}

// SIGNAL 9
void ModbusDevice::pollingStopped()
{
    QMetaObject::activate(this, &staticMetaObject, 9, nullptr);
}

// SIGNAL 10
void ModbusDevice::statisticsUpdated()
{
    QMetaObject::activate(this, &staticMetaObject, 10, nullptr);
}

// SIGNAL 11
void ModbusDevice::configurationChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 11, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
