/****************************************************************************
** Meta object code from reading C++ file 'mygl.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.2.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../assignment_package/src/mygl.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mygl.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.2.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MyGL_t {
    const uint offsetsAndSize[46];
    char stringdata0[204];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_MyGL_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_MyGL_t qt_meta_stringdata_MyGL = {
    {
QT_MOC_LITERAL(0, 4), // "MyGL"
QT_MOC_LITERAL(5, 10), // "slot_setVX"
QT_MOC_LITERAL(16, 0), // ""
QT_MOC_LITERAL(17, 1), // "x"
QT_MOC_LITERAL(19, 10), // "slot_setVY"
QT_MOC_LITERAL(30, 10), // "slot_setVZ"
QT_MOC_LITERAL(41, 9), // "slot_setR"
QT_MOC_LITERAL(51, 1), // "a"
QT_MOC_LITERAL(53, 9), // "slot_setG"
QT_MOC_LITERAL(63, 9), // "slot_setB"
QT_MOC_LITERAL(73, 10), // "slot_setJX"
QT_MOC_LITERAL(84, 10), // "slot_setJY"
QT_MOC_LITERAL(95, 10), // "slot_setJZ"
QT_MOC_LITERAL(106, 21), // "slot_setSelectedJoint"
QT_MOC_LITERAL(128, 16), // "QTreeWidgetItem*"
QT_MOC_LITERAL(145, 10), // "helperDraw"
QT_MOC_LITERAL(156, 6), // "Joint*"
QT_MOC_LITERAL(163, 5), // "joint"
QT_MOC_LITERAL(169, 8), // "skinMesh"
QT_MOC_LITERAL(178, 10), // "helperSkin"
QT_MOC_LITERAL(189, 7), // "vertex*"
QT_MOC_LITERAL(197, 4), // "vert"
QT_MOC_LITERAL(202, 1) // "j"

    },
    "MyGL\0slot_setVX\0\0x\0slot_setVY\0slot_setVZ\0"
    "slot_setR\0a\0slot_setG\0slot_setB\0"
    "slot_setJX\0slot_setJY\0slot_setJZ\0"
    "slot_setSelectedJoint\0QTreeWidgetItem*\0"
    "helperDraw\0Joint*\0joint\0skinMesh\0"
    "helperSkin\0vertex*\0vert\0j"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MyGL[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   92,    2, 0x0a,    1 /* Public */,
       4,    1,   95,    2, 0x0a,    3 /* Public */,
       5,    1,   98,    2, 0x0a,    5 /* Public */,
       6,    1,  101,    2, 0x0a,    7 /* Public */,
       8,    1,  104,    2, 0x0a,    9 /* Public */,
       9,    1,  107,    2, 0x0a,   11 /* Public */,
      10,    1,  110,    2, 0x0a,   13 /* Public */,
      11,    1,  113,    2, 0x0a,   15 /* Public */,
      12,    1,  116,    2, 0x0a,   17 /* Public */,
      13,    1,  119,    2, 0x0a,   19 /* Public */,
      15,    1,  122,    2, 0x0a,   21 /* Public */,
      18,    0,  125,    2, 0x0a,   23 /* Public */,
      19,    2,  126,    2, 0x0a,   24 /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Double,    3,
    QMetaType::Void, QMetaType::Double,    3,
    QMetaType::Void, QMetaType::Double,    3,
    QMetaType::Void, QMetaType::Double,    7,
    QMetaType::Void, QMetaType::Double,    7,
    QMetaType::Void, QMetaType::Double,    7,
    QMetaType::Void, QMetaType::Double,    3,
    QMetaType::Void, QMetaType::Double,    3,
    QMetaType::Void, QMetaType::Double,    3,
    QMetaType::Void, 0x80000000 | 14,    2,
    QMetaType::Void, 0x80000000 | 16,   17,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 20, 0x80000000 | 16,   21,   22,

       0        // eod
};

void MyGL::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MyGL *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->slot_setVX((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 1: _t->slot_setVY((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 2: _t->slot_setVZ((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 3: _t->slot_setR((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 4: _t->slot_setG((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 5: _t->slot_setB((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 6: _t->slot_setJX((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 7: _t->slot_setJY((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 8: _t->slot_setJZ((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 9: _t->slot_setSelectedJoint((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1]))); break;
        case 10: _t->helperDraw((*reinterpret_cast< Joint*(*)>(_a[1]))); break;
        case 11: _t->skinMesh(); break;
        case 12: _t->helperSkin((*reinterpret_cast< vertex*(*)>(_a[1])),(*reinterpret_cast< Joint*(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObject MyGL::staticMetaObject = { {
    QMetaObject::SuperData::link<OpenGLContext::staticMetaObject>(),
    qt_meta_stringdata_MyGL.offsetsAndSize,
    qt_meta_data_MyGL,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_MyGL_t
, QtPrivate::TypeAndForceComplete<MyGL, std::true_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<double, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<double, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<double, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<double, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<double, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<double, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<double, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<double, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<double, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QTreeWidgetItem *, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<Joint *, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<vertex *, std::false_type>, QtPrivate::TypeAndForceComplete<Joint *, std::false_type>


>,
    nullptr
} };


const QMetaObject *MyGL::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MyGL::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MyGL.stringdata0))
        return static_cast<void*>(this);
    return OpenGLContext::qt_metacast(_clname);
}

int MyGL::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = OpenGLContext::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 13;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
