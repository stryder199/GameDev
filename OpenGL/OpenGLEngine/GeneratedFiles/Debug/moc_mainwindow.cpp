/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created: Sun Apr 7 00:24:33 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/mainwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainWindow[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      19,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x0a,
      40,   25,   11,   11, 0x0a,
      70,   25,   11,   11, 0x0a,
      99,   11,   11,   11, 0x0a,
     113,   11,   11,   11, 0x0a,
     130,   11,   11,   11, 0x08,
     161,   11,   11,   11, 0x08,
     210,  198,   11,   11, 0x08,
     230,  198,   11,   11, 0x08,
     250,  198,   11,   11, 0x08,
     270,  198,   11,   11, 0x08,
     291,  198,   11,   11, 0x08,
     312,  198,   11,   11, 0x08,
     333,  198,   11,   11, 0x08,
     357,  198,   11,   11, 0x08,
     381,  198,   11,   11, 0x08,
     411,  405,   11,   11, 0x08,
     425,  405,   11,   11, 0x08,
     438,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0drawOpenGL()\0m_GeometryName\0"
    "newGeometryAdded(std::string)\0"
    "geometryRemoved(std::string)\0addGeometry()\0"
    "removeGeometry()\0on_actionAdd_Model_triggered()\0"
    "on_actionRemove_Selected_triggered()\0"
    "sliderValue\0scaleGeometryX(int)\0"
    "scaleGeometryY(int)\0scaleGeometryZ(int)\0"
    "rotateGeometryX(int)\0rotateGeometryY(int)\0"
    "rotateGeometryZ(int)\0translateGeometryX(int)\0"
    "translateGeometryY(int)\0translateGeometryZ(int)\0"
    "state\0setOrtho(int)\0setPost(int)\0"
    "addTraceDialog()\0"
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MainWindow *_t = static_cast<MainWindow *>(_o);
        switch (_id) {
        case 0: _t->drawOpenGL(); break;
        case 1: _t->newGeometryAdded((*reinterpret_cast< std::string(*)>(_a[1]))); break;
        case 2: _t->geometryRemoved((*reinterpret_cast< std::string(*)>(_a[1]))); break;
        case 3: _t->addGeometry(); break;
        case 4: _t->removeGeometry(); break;
        case 5: _t->on_actionAdd_Model_triggered(); break;
        case 6: _t->on_actionRemove_Selected_triggered(); break;
        case 7: _t->scaleGeometryX((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->scaleGeometryY((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->scaleGeometryZ((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->rotateGeometryX((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->rotateGeometryY((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: _t->rotateGeometryZ((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: _t->translateGeometryX((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 14: _t->translateGeometryY((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 15: _t->translateGeometryZ((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 16: _t->setOrtho((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 17: _t->setPost((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 18: _t->addTraceDialog(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData MainWindow::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow,
      qt_meta_data_MainWindow, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MainWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 19)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 19;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
