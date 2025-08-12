/****************************************************************************
** Meta object code from reading C++ file 'ModbusConnection.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.13)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "src/core/ModbusConnection.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ModbusConnection.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.13. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ModbusConnection_t {
    QByteArrayData data[15];
    char stringdata0[201];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ModbusConnection_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ModbusConnection_t qt_meta_stringdata_ModbusConnection = {
    {
QT_MOC_LITERAL(0, 0, 16), // "ModbusConnection"
QT_MOC_LITERAL(1, 17, 15), // "deviceConnected"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 18), // "deviceDisconnected"
QT_MOC_LITERAL(4, 53, 15), // "connectionError"
QT_MOC_LITERAL(5, 69, 5), // "error"
QT_MOC_LITERAL(6, 75, 18), // "communicationError"
QT_MOC_LITERAL(7, 94, 16), // "requestCompleted"
QT_MOC_LITERAL(8, 111, 7), // "success"
QT_MOC_LITERAL(9, 119, 12), // "debugMessage"
QT_MOC_LITERAL(10, 132, 7), // "message"
QT_MOC_LITERAL(11, 140, 17), // "statisticsUpdated"
QT_MOC_LITERAL(12, 158, 12), // "processQueue"
QT_MOC_LITERAL(13, 171, 13), // "handleTimeout"
QT_MOC_LITERAL(14, 185, 15) // "checkConnection"

    },
    "ModbusConnection\0deviceConnected\0\0"
    "deviceDisconnected\0connectionError\0"
    "error\0communicationError\0requestCompleted\0"
    "success\0debugMessage\0message\0"
    "statisticsUpdated\0processQueue\0"
    "handleTimeout\0checkConnection"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ModbusConnection[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       7,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   64,    2, 0x06 /* Public */,
       3,    0,   65,    2, 0x06 /* Public */,
       4,    1,   66,    2, 0x06 /* Public */,
       6,    1,   69,    2, 0x06 /* Public */,
       7,    1,   72,    2, 0x06 /* Public */,
       9,    1,   75,    2, 0x06 /* Public */,
      11,    0,   78,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      12,    0,   79,    2, 0x08 /* Private */,
      13,    0,   80,    2, 0x08 /* Private */,
      14,    0,   81,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::Bool,    8,
    QMetaType::Void, QMetaType::QString,   10,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void ModbusConnection::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ModbusConnection *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->deviceConnected(); break;
        case 1: _t->deviceDisconnected(); break;
        case 2: _t->connectionError((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->communicationError((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: _t->requestCompleted((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->debugMessage((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 6: _t->statisticsUpdated(); break;
        case 7: _t->processQueue(); break;
        case 8: _t->handleTimeout(); break;
        case 9: _t->checkConnection(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ModbusConnection::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ModbusConnection::deviceConnected)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ModbusConnection::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ModbusConnection::deviceDisconnected)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (ModbusConnection::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ModbusConnection::connectionError)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (ModbusConnection::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ModbusConnection::communicationError)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (ModbusConnection::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ModbusConnection::requestCompleted)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (ModbusConnection::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ModbusConnection::debugMessage)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (ModbusConnection::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ModbusConnection::statisticsUpdated)) {
                *result = 6;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ModbusConnection::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_ModbusConnection.data,
    qt_meta_data_ModbusConnection,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ModbusConnection::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ModbusConnection::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ModbusConnection.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "IModbus"))
        return static_cast< IModbus*>(this);
    return QObject::qt_metacast(_clname);
}

int ModbusConnection::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void ModbusConnection::deviceConnected()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void ModbusConnection::deviceDisconnected()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void ModbusConnection::connectionError(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void ModbusConnection::communicationError(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void ModbusConnection::requestCompleted(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void ModbusConnection::debugMessage(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void ModbusConnection::statisticsUpdated()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
