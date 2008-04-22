/****************************************************************************
** Meta object code from reading C++ file 'CImageViewComp.h'
**
** Created: Tue 22. Apr 10:58:31 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.2.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../CImageViewComp.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CImageViewComp.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.2.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_iqt2d__CImageViewComp[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      23,   22,   22,   22, 0x05,

 // slots: signature, parameters, type, tag, flags
      55,   43,   22,   22, 0x0a,
      82,   71,   22,   22, 0x0a,
      99,   22,   22,   22, 0x0a,
     117,   22,   22,   22, 0x0a,
     135,   22,   22,   22, 0x0a,
     154,   22,   22,   22, 0x0a,
     168,   22,   22,   22, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_iqt2d__CImageViewComp[] = {
    "iqt2d::CImageViewComp\0\0zoomChanged(double)\0"
    "scaleFactor\0SetZoom(double)\0zoomString\0"
    "SetZoom(QString)\0OnZoomIncrement()\0"
    "OnZoomDecrement()\0SwitchFullScreen()\0"
    "OnFitInView()\0OnFitToImage()\0"
};

const QMetaObject iqt2d::CImageViewComp::staticMetaObject = {
    { &iqt::TGuiObserverWrap<iqt::TGuiComponentBase<QGraphicsView>,imod::TSingleModelObserverBase<iimg::IBitmap> >::staticMetaObject, qt_meta_stringdata_iqt2d__CImageViewComp,
      qt_meta_data_iqt2d__CImageViewComp, 0 }
};

const QMetaObject *iqt2d::CImageViewComp::metaObject() const
{
    return &staticMetaObject;
}

void *iqt2d::CImageViewComp::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_iqt2d__CImageViewComp))
	return static_cast<void*>(const_cast< CImageViewComp*>(this));
    typedef iqt::TGuiObserverWrap<iqt::TGuiComponentBase<QGraphicsView>,imod::TSingleModelObserverBase<iimg::IBitmap> > QMocSuperClass;
    return QMocSuperClass::qt_metacast(_clname);
}

int iqt2d::CImageViewComp::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    typedef iqt::TGuiObserverWrap<iqt::TGuiComponentBase<QGraphicsView>,imod::TSingleModelObserverBase<iimg::IBitmap> > QMocSuperClass;
    _id = QMocSuperClass::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: zoomChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 1: SetZoom((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 2: SetZoom((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: OnZoomIncrement(); break;
        case 4: OnZoomDecrement(); break;
        case 5: SwitchFullScreen(); break;
        case 6: OnFitInView(); break;
        case 7: OnFitToImage(); break;
        }
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void iqt2d::CImageViewComp::zoomChanged(double _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
