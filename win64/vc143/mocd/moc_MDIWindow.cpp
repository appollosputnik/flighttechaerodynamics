/****************************************************************************
** Meta object code from reading C++ file 'MDIWindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../Common/src/MDIWindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MDIWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MDIWindow_t {
    QByteArrayData data[22];
    char stringdata0[343];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MDIWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MDIWindow_t qt_meta_stringdata_MDIWindow = {
    {
QT_MOC_LITERAL(0, 0, 9), // "MDIWindow"
QT_MOC_LITERAL(1, 10, 16), // "selectionChanged"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 7), // "message"
QT_MOC_LITERAL(4, 36, 13), // "sendCloseView"
QT_MOC_LITERAL(5, 50, 10), // "MDIWindow*"
QT_MOC_LITERAL(6, 61, 7), // "theView"
QT_MOC_LITERAL(7, 69, 21), // "func_label_text_color"
QT_MOC_LITERAL(8, 91, 32), // "func_background_color_controller"
QT_MOC_LITERAL(9, 124, 23), // "func_LineEditColor_text"
QT_MOC_LITERAL(10, 148, 27), // "func_LineditBackgroundColor"
QT_MOC_LITERAL(11, 176, 16), // "func_PBTextcolor"
QT_MOC_LITERAL(12, 193, 18), // "func_PB_Background"
QT_MOC_LITERAL(13, 212, 28), // "func_consoleColor_background"
QT_MOC_LITERAL(14, 241, 22), // "func_consoleColor_text"
QT_MOC_LITERAL(15, 264, 10), // "closeEvent"
QT_MOC_LITERAL(16, 275, 12), // "QCloseEvent*"
QT_MOC_LITERAL(17, 288, 1), // "e"
QT_MOC_LITERAL(18, 290, 17), // "onWindowActivated"
QT_MOC_LITERAL(19, 308, 4), // "dump"
QT_MOC_LITERAL(20, 313, 15), // "func_draw_wings"
QT_MOC_LITERAL(21, 329, 13) // "_show_console"

    },
    "MDIWindow\0selectionChanged\0\0message\0"
    "sendCloseView\0MDIWindow*\0theView\0"
    "func_label_text_color\0"
    "func_background_color_controller\0"
    "func_LineEditColor_text\0"
    "func_LineditBackgroundColor\0"
    "func_PBTextcolor\0func_PB_Background\0"
    "func_consoleColor_background\0"
    "func_consoleColor_text\0closeEvent\0"
    "QCloseEvent*\0e\0onWindowActivated\0dump\0"
    "func_draw_wings\0_show_console"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MDIWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   94,    2, 0x06 /* Public */,
       3,    2,   95,    2, 0x06 /* Public */,
       4,    1,  100,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    0,  103,    2, 0x0a /* Public */,
       8,    0,  104,    2, 0x0a /* Public */,
       9,    0,  105,    2, 0x0a /* Public */,
      10,    0,  106,    2, 0x0a /* Public */,
      11,    0,  107,    2, 0x0a /* Public */,
      12,    0,  108,    2, 0x0a /* Public */,
      13,    0,  109,    2, 0x0a /* Public */,
      14,    0,  110,    2, 0x0a /* Public */,
      15,    1,  111,    2, 0x0a /* Public */,
      18,    0,  114,    2, 0x0a /* Public */,
      19,    0,  115,    2, 0x0a /* Public */,
      20,    0,  116,    2, 0x0a /* Public */,
      21,    0,  117,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    2,    2,
    QMetaType::Void, 0x80000000 | 5,    6,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 16,   17,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MDIWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MDIWindow *_t = static_cast<MDIWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->selectionChanged(); break;
        case 1: _t->message((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: _t->sendCloseView((*reinterpret_cast< MDIWindow*(*)>(_a[1]))); break;
        case 3: _t->func_label_text_color(); break;
        case 4: _t->func_background_color_controller(); break;
        case 5: _t->func_LineEditColor_text(); break;
        case 6: _t->func_LineditBackgroundColor(); break;
        case 7: _t->func_PBTextcolor(); break;
        case 8: _t->func_PB_Background(); break;
        case 9: _t->func_consoleColor_background(); break;
        case 10: _t->func_consoleColor_text(); break;
        case 11: _t->closeEvent((*reinterpret_cast< QCloseEvent*(*)>(_a[1]))); break;
        case 12: _t->onWindowActivated(); break;
        case 13: _t->dump(); break;
        case 14: _t->func_draw_wings(); break;
        case 15: _t->_show_console(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< MDIWindow* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (MDIWindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MDIWindow::selectionChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (MDIWindow::*)(const QString & , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MDIWindow::message)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (MDIWindow::*)(MDIWindow * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MDIWindow::sendCloseView)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MDIWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MDIWindow.data,
      qt_meta_data_MDIWindow,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *MDIWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MDIWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MDIWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MDIWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    }
    return _id;
}

// SIGNAL 0
void MDIWindow::selectionChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void MDIWindow::message(const QString & _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void MDIWindow::sendCloseView(MDIWindow * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
