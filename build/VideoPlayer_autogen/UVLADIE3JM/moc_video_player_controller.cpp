/****************************************************************************
** Meta object code from reading C++ file 'video_player_controller.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/video_player_controller.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'video_player_controller.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_VideoPlayerController_t {
    QByteArrayData data[8];
    char stringdata0[74];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_VideoPlayerController_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_VideoPlayerController_t qt_meta_stringdata_VideoPlayerController = {
    {
QT_MOC_LITERAL(0, 0, 21), // "VideoPlayerController"
QT_MOC_LITERAL(1, 22, 10), // "frameReady"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 13), // "errorOccurred"
QT_MOC_LITERAL(4, 48, 11), // "errorString"
QT_MOC_LITERAL(5, 60, 4), // "play"
QT_MOC_LITERAL(6, 65, 3), // "url"
QT_MOC_LITERAL(7, 69, 4) // "stop"

    },
    "VideoPlayerController\0frameReady\0\0"
    "errorOccurred\0errorString\0play\0url\0"
    "stop"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_VideoPlayerController[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x06 /* Public */,
       3,    1,   35,    2, 0x06 /* Public */,

 // methods: name, argc, parameters, tag, flags
       5,    1,   38,    2, 0x02 /* Public */,
       7,    0,   41,    2, 0x02 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    4,

 // methods: parameters
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void,

       0        // eod
};

void VideoPlayerController::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<VideoPlayerController *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->frameReady(); break;
        case 1: _t->errorOccurred((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->play((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->stop(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (VideoPlayerController::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoPlayerController::frameReady)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (VideoPlayerController::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoPlayerController::errorOccurred)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject VideoPlayerController::staticMetaObject = { {
    QMetaObject::SuperData::link<QThread::staticMetaObject>(),
    qt_meta_stringdata_VideoPlayerController.data,
    qt_meta_data_VideoPlayerController,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *VideoPlayerController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *VideoPlayerController::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_VideoPlayerController.stringdata0))
        return static_cast<void*>(this);
    return QThread::qt_metacast(_clname);
}

int VideoPlayerController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void VideoPlayerController::frameReady()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void VideoPlayerController::errorOccurred(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
