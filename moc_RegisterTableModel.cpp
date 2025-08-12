/****************************************************************************
** Meta object code from reading C++ file 'RegisterTableModel.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.13)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "src/ui/RegisterTableModel.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'RegisterTableModel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.13. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_RegisterTableModel_t {
    QByteArrayData data[28];
    char stringdata0[322];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RegisterTableModel_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RegisterTableModel_t qt_meta_stringdata_RegisterTableModel = {
    {
QT_MOC_LITERAL(0, 0, 18), // "RegisterTableModel"
QT_MOC_LITERAL(1, 19, 13), // "registerAdded"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 7), // "address"
QT_MOC_LITERAL(4, 42, 15), // "registerRemoved"
QT_MOC_LITERAL(5, 58, 15), // "registerUpdated"
QT_MOC_LITERAL(6, 74, 12), // "valueChanged"
QT_MOC_LITERAL(7, 87, 5), // "value"
QT_MOC_LITERAL(8, 93, 17), // "alarmStateChanged"
QT_MOC_LITERAL(9, 111, 7), // "inAlarm"
QT_MOC_LITERAL(10, 119, 13), // "formatChanged"
QT_MOC_LITERAL(11, 133, 13), // "DisplayFormat"
QT_MOC_LITERAL(12, 147, 6), // "format"
QT_MOC_LITERAL(13, 154, 5), // "error"
QT_MOC_LITERAL(14, 160, 7), // "message"
QT_MOC_LITERAL(15, 168, 20), // "configurationChanged"
QT_MOC_LITERAL(16, 189, 22), // "onRegisterValueChanged"
QT_MOC_LITERAL(17, 212, 27), // "onRegisterAlarmStateChanged"
QT_MOC_LITERAL(18, 240, 23), // "onRegisterConfigChanged"
QT_MOC_LITERAL(19, 264, 15), // "onFormatChanged"
QT_MOC_LITERAL(20, 280, 3), // "DEC"
QT_MOC_LITERAL(21, 284, 3), // "HEX"
QT_MOC_LITERAL(22, 288, 3), // "BIN"
QT_MOC_LITERAL(23, 292, 5), // "FLOAT"
QT_MOC_LITERAL(24, 298, 6), // "DOUBLE"
QT_MOC_LITERAL(25, 305, 4), // "WORD"
QT_MOC_LITERAL(26, 310, 5), // "DWORD"
QT_MOC_LITERAL(27, 316, 5) // "ASCII"

    },
    "RegisterTableModel\0registerAdded\0\0"
    "address\0registerRemoved\0registerUpdated\0"
    "valueChanged\0value\0alarmStateChanged\0"
    "inAlarm\0formatChanged\0DisplayFormat\0"
    "format\0error\0message\0configurationChanged\0"
    "onRegisterValueChanged\0"
    "onRegisterAlarmStateChanged\0"
    "onRegisterConfigChanged\0onFormatChanged\0"
    "DEC\0HEX\0BIN\0FLOAT\0DOUBLE\0WORD\0DWORD\0"
    "ASCII"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RegisterTableModel[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       1,  120, // enums/sets
       0,    0, // constructors
       0,       // flags
       8,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   74,    2, 0x06 /* Public */,
       4,    1,   77,    2, 0x06 /* Public */,
       5,    1,   80,    2, 0x06 /* Public */,
       6,    2,   83,    2, 0x06 /* Public */,
       8,    2,   88,    2, 0x06 /* Public */,
      10,    2,   93,    2, 0x06 /* Public */,
      13,    1,   98,    2, 0x06 /* Public */,
      15,    0,  101,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      16,    2,  102,    2, 0x0a /* Public */,
      17,    2,  107,    2, 0x0a /* Public */,
      18,    1,  112,    2, 0x0a /* Public */,
      19,    2,  115,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int, QMetaType::QVariant,    3,    7,
    QMetaType::Void, QMetaType::Int, QMetaType::Bool,    3,    9,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 11,    3,   12,
    QMetaType::Void, QMetaType::QString,   14,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::QVariant,    3,    7,
    QMetaType::Void, QMetaType::Int, QMetaType::Bool,    3,    9,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 11,    3,   12,

 // enums: name, alias, flags, count, data
      11,   11, 0x2,    8,  125,

 // enum data: key, value
      20, uint(RegisterTableModel::DisplayFormat::DEC),
      21, uint(RegisterTableModel::DisplayFormat::HEX),
      22, uint(RegisterTableModel::DisplayFormat::BIN),
      23, uint(RegisterTableModel::DisplayFormat::FLOAT),
      24, uint(RegisterTableModel::DisplayFormat::DOUBLE),
      25, uint(RegisterTableModel::DisplayFormat::WORD),
      26, uint(RegisterTableModel::DisplayFormat::DWORD),
      27, uint(RegisterTableModel::DisplayFormat::ASCII),

       0        // eod
};

void RegisterTableModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<RegisterTableModel *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->registerAdded((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->registerRemoved((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->registerUpdated((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->valueChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2]))); break;
        case 4: _t->alarmStateChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 5: _t->formatChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< DisplayFormat(*)>(_a[2]))); break;
        case 6: _t->error((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 7: _t->configurationChanged(); break;
        case 8: _t->onRegisterValueChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2]))); break;
        case 9: _t->onRegisterAlarmStateChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 10: _t->onRegisterConfigChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->onFormatChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< DisplayFormat(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (RegisterTableModel::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RegisterTableModel::registerAdded)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (RegisterTableModel::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RegisterTableModel::registerRemoved)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (RegisterTableModel::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RegisterTableModel::registerUpdated)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (RegisterTableModel::*)(int , const QVariant & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RegisterTableModel::valueChanged)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (RegisterTableModel::*)(int , bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RegisterTableModel::alarmStateChanged)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (RegisterTableModel::*)(int , DisplayFormat );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RegisterTableModel::formatChanged)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (RegisterTableModel::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RegisterTableModel::error)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (RegisterTableModel::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RegisterTableModel::configurationChanged)) {
                *result = 7;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject RegisterTableModel::staticMetaObject = { {
    QMetaObject::SuperData::link<QAbstractTableModel::staticMetaObject>(),
    qt_meta_stringdata_RegisterTableModel.data,
    qt_meta_data_RegisterTableModel,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *RegisterTableModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RegisterTableModel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_RegisterTableModel.stringdata0))
        return static_cast<void*>(this);
    return QAbstractTableModel::qt_metacast(_clname);
}

int RegisterTableModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractTableModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void RegisterTableModel::registerAdded(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void RegisterTableModel::registerRemoved(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void RegisterTableModel::registerUpdated(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void RegisterTableModel::valueChanged(int _t1, const QVariant & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void RegisterTableModel::alarmStateChanged(int _t1, bool _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void RegisterTableModel::formatChanged(int _t1, DisplayFormat _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void RegisterTableModel::error(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void RegisterTableModel::configurationChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
