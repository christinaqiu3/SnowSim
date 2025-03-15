/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.2.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../assignment_package/src/mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.2.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    const uint offsetsAndSize[86];
    char stringdata0[867];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 10), // "MainWindow"
QT_MOC_LITERAL(11, 12), // "sig_sendVert"
QT_MOC_LITERAL(24, 0), // ""
QT_MOC_LITERAL(25, 16), // "QListWidgetItem*"
QT_MOC_LITERAL(42, 12), // "selectedItem"
QT_MOC_LITERAL(55, 16), // "sig_sendHalfEdge"
QT_MOC_LITERAL(72, 12), // "sig_sendFace"
QT_MOC_LITERAL(85, 13), // "sig_sendJoint"
QT_MOC_LITERAL(99, 16), // "QTreeWidgetItem*"
QT_MOC_LITERAL(116, 23), // "on_actionQuit_triggered"
QT_MOC_LITERAL(140, 20), // "on_chooseOBJ_clicked"
QT_MOC_LITERAL(161, 22), // "on_addMidpoint_clicked"
QT_MOC_LITERAL(184, 26), // "on_triangulateFace_clicked"
QT_MOC_LITERAL(211, 23), // "on_catmullClark_clicked"
QT_MOC_LITERAL(235, 19), // "on_loadJSON_clicked"
QT_MOC_LITERAL(255, 20), // "on_exportUSD_clicked"
QT_MOC_LITERAL(276, 22), // "on_changeColor_clicked"
QT_MOC_LITERAL(299, 24), // "slot_addRootToTreeWidget"
QT_MOC_LITERAL(324, 24), // "on_jointRotXPlus_clicked"
QT_MOC_LITERAL(349, 25), // "on_jointRotXMinus_clicked"
QT_MOC_LITERAL(375, 24), // "on_jointRotYPlus_clicked"
QT_MOC_LITERAL(400, 25), // "on_jointRotYMinus_clicked"
QT_MOC_LITERAL(426, 24), // "on_jointRotZPlus_clicked"
QT_MOC_LITERAL(451, 25), // "on_jointRotZMinus_clicked"
QT_MOC_LITERAL(477, 23), // "on_meshSkinning_clicked"
QT_MOC_LITERAL(501, 19), // "smoothEdgeMidpoints"
QT_MOC_LITERAL(521, 37), // "std::unordered_map<halfedge*,..."
QT_MOC_LITERAL(559, 33), // "std::unordered_map<face*,vert..."
QT_MOC_LITERAL(593, 13), // "faceCentroids"
QT_MOC_LITERAL(607, 16), // "numOrigHalfEdges"
QT_MOC_LITERAL(624, 22), // "smoothOriginalVertices"
QT_MOC_LITERAL(647, 12), // "numOrigVerts"
QT_MOC_LITERAL(660, 13), // "Quadrangulate"
QT_MOC_LITERAL(674, 5), // "face*"
QT_MOC_LITERAL(680, 12), // "originalFace"
QT_MOC_LITERAL(693, 7), // "vertex*"
QT_MOC_LITERAL(701, 8), // "centroid"
QT_MOC_LITERAL(710, 34), // "on_actionCamera_Controls_trig..."
QT_MOC_LITERAL(745, 13), // "populateLists"
QT_MOC_LITERAL(759, 26), // "updateMeshComponentWidgetV"
QT_MOC_LITERAL(786, 26), // "updateMeshComponentWidgetH"
QT_MOC_LITERAL(813, 26), // "updateMeshComponentWidgetF"
QT_MOC_LITERAL(840, 26) // "updateJointComponentWidget"

    },
    "MainWindow\0sig_sendVert\0\0QListWidgetItem*\0"
    "selectedItem\0sig_sendHalfEdge\0"
    "sig_sendFace\0sig_sendJoint\0QTreeWidgetItem*\0"
    "on_actionQuit_triggered\0on_chooseOBJ_clicked\0"
    "on_addMidpoint_clicked\0"
    "on_triangulateFace_clicked\0"
    "on_catmullClark_clicked\0on_loadJSON_clicked\0"
    "on_exportUSD_clicked\0on_changeColor_clicked\0"
    "slot_addRootToTreeWidget\0"
    "on_jointRotXPlus_clicked\0"
    "on_jointRotXMinus_clicked\0"
    "on_jointRotYPlus_clicked\0"
    "on_jointRotYMinus_clicked\0"
    "on_jointRotZPlus_clicked\0"
    "on_jointRotZMinus_clicked\0"
    "on_meshSkinning_clicked\0smoothEdgeMidpoints\0"
    "std::unordered_map<halfedge*,vertex*>\0"
    "std::unordered_map<face*,vertex*>\0"
    "faceCentroids\0numOrigHalfEdges\0"
    "smoothOriginalVertices\0numOrigVerts\0"
    "Quadrangulate\0face*\0originalFace\0"
    "vertex*\0centroid\0on_actionCamera_Controls_triggered\0"
    "populateLists\0updateMeshComponentWidgetV\0"
    "updateMeshComponentWidgetH\0"
    "updateMeshComponentWidgetF\0"
    "updateJointComponentWidget"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
      29,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,  188,    2, 0x06,    1 /* Public */,
       5,    1,  191,    2, 0x06,    3 /* Public */,
       6,    1,  194,    2, 0x06,    5 /* Public */,
       7,    1,  197,    2, 0x06,    7 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       9,    0,  200,    2, 0x0a,    9 /* Public */,
      10,    0,  201,    2, 0x0a,   10 /* Public */,
      11,    0,  202,    2, 0x0a,   11 /* Public */,
      12,    0,  203,    2, 0x0a,   12 /* Public */,
      13,    0,  204,    2, 0x0a,   13 /* Public */,
      14,    0,  205,    2, 0x0a,   14 /* Public */,
      15,    0,  206,    2, 0x0a,   15 /* Public */,
      16,    0,  207,    2, 0x0a,   16 /* Public */,
      17,    1,  208,    2, 0x0a,   17 /* Public */,
      18,    0,  211,    2, 0x0a,   19 /* Public */,
      19,    0,  212,    2, 0x0a,   20 /* Public */,
      20,    0,  213,    2, 0x0a,   21 /* Public */,
      21,    0,  214,    2, 0x0a,   22 /* Public */,
      22,    0,  215,    2, 0x0a,   23 /* Public */,
      23,    0,  216,    2, 0x0a,   24 /* Public */,
      24,    0,  217,    2, 0x0a,   25 /* Public */,
      25,    2,  218,    2, 0x0a,   26 /* Public */,
      30,    2,  223,    2, 0x0a,   29 /* Public */,
      32,    2,  228,    2, 0x0a,   32 /* Public */,
      37,    0,  233,    2, 0x0a,   35 /* Public */,
      38,    0,  234,    2, 0x0a,   36 /* Public */,
      39,    1,  235,    2, 0x0a,   37 /* Public */,
      40,    1,  238,    2, 0x0a,   39 /* Public */,
      41,    1,  241,    2, 0x0a,   41 /* Public */,
      42,    1,  244,    2, 0x0a,   43 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 8,    4,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 8,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    0x80000000 | 26, 0x80000000 | 27, QMetaType::Int,   28,   29,
    QMetaType::Void, 0x80000000 | 27, QMetaType::Int,   28,   31,
    QMetaType::Void, 0x80000000 | 33, 0x80000000 | 35,   34,   36,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 8,    4,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->sig_sendVert((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 1: _t->sig_sendHalfEdge((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 2: _t->sig_sendFace((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 3: _t->sig_sendJoint((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1]))); break;
        case 4: _t->on_actionQuit_triggered(); break;
        case 5: _t->on_chooseOBJ_clicked(); break;
        case 6: _t->on_addMidpoint_clicked(); break;
        case 7: _t->on_triangulateFace_clicked(); break;
        case 8: _t->on_catmullClark_clicked(); break;
        case 9: _t->on_loadJSON_clicked(); break;
        case 10: _t->on_exportUSD_clicked(); break;
        case 11: _t->on_changeColor_clicked(); break;
        case 12: _t->slot_addRootToTreeWidget((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1]))); break;
        case 13: _t->on_jointRotXPlus_clicked(); break;
        case 14: _t->on_jointRotXMinus_clicked(); break;
        case 15: _t->on_jointRotYPlus_clicked(); break;
        case 16: _t->on_jointRotYMinus_clicked(); break;
        case 17: _t->on_jointRotZPlus_clicked(); break;
        case 18: _t->on_jointRotZMinus_clicked(); break;
        case 19: _t->on_meshSkinning_clicked(); break;
        case 20: { std::unordered_map<halfedge*,vertex*> _r = _t->smoothEdgeMidpoints((*reinterpret_cast< std::unordered_map<face*,vertex*>(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< std::unordered_map<halfedge*,vertex*>*>(_a[0]) = std::move(_r); }  break;
        case 21: _t->smoothOriginalVertices((*reinterpret_cast< std::unordered_map<face*,vertex*>(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 22: _t->Quadrangulate((*reinterpret_cast< face*(*)>(_a[1])),(*reinterpret_cast< vertex*(*)>(_a[2]))); break;
        case 23: _t->on_actionCamera_Controls_triggered(); break;
        case 24: _t->populateLists(); break;
        case 25: _t->updateMeshComponentWidgetV((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 26: _t->updateMeshComponentWidgetH((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 27: _t->updateMeshComponentWidgetF((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 28: _t->updateJointComponentWidget((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (MainWindow::*)(QListWidgetItem * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWindow::sig_sendVert)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (MainWindow::*)(QListWidgetItem * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWindow::sig_sendHalfEdge)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (MainWindow::*)(QListWidgetItem * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWindow::sig_sendFace)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (MainWindow::*)(QTreeWidgetItem * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWindow::sig_sendJoint)) {
                *result = 3;
                return;
            }
        }
    }
}

const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_MainWindow.offsetsAndSize,
    qt_meta_data_MainWindow,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_MainWindow_t
, QtPrivate::TypeAndForceComplete<MainWindow, std::true_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QListWidgetItem *, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QListWidgetItem *, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QListWidgetItem *, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QTreeWidgetItem *, std::false_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QTreeWidgetItem *, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<std::unordered_map<halfedge*,vertex*>, std::false_type>, QtPrivate::TypeAndForceComplete<std::unordered_map<face*,vertex*>, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<std::unordered_map<face*,vertex*>, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<face *, std::false_type>, QtPrivate::TypeAndForceComplete<vertex *, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QListWidgetItem *, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QListWidgetItem *, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QListWidgetItem *, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QTreeWidgetItem *, std::false_type>


>,
    nullptr
} };


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 29)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 29;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 29)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 29;
    }
    return _id;
}

// SIGNAL 0
void MainWindow::sig_sendVert(QListWidgetItem * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MainWindow::sig_sendHalfEdge(QListWidgetItem * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void MainWindow::sig_sendFace(QListWidgetItem * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void MainWindow::sig_sendJoint(QTreeWidgetItem * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
