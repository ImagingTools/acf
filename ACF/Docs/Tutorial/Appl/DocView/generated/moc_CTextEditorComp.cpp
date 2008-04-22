/****************************************************************************
** Meta object code from reading C++ file 'CTextEditorComp.h'
**
** Created: Tue 22. Apr 11:00:12 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.2.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../CTextEditorComp.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CTextEditorComp.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.2.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_CTextEditorComp[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      17,   16,   16,   16, 0x09,
      38,   16,   16,   16, 0x09,
      54,   16,   16,   16, 0x09,
      70,   16,   16,   16, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_CTextEditorComp[] = {
    "CTextEditorComp\0\0OnSelectionChanged()\0"
    "OnTextChanged()\0OnToLowercase()\0"
    "OnToUppercase()\0"
};

const QMetaObject CTextEditorComp::staticMetaObject = {
    { &iqt::TGuiObserverWrap<iqt::TGuiComponentBase<QTextEdit>,imod::TSingleModelObserverBase<IText> >::staticMetaObject, qt_meta_stringdata_CTextEditorComp,
      qt_meta_data_CTextEditorComp, 0 }
};

const QMetaObject *CTextEditorComp::metaObject() const
{
    return &staticMetaObject;
}

void *CTextEditorComp::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CTextEditorComp))
	return static_cast<void*>(const_cast< CTextEditorComp*>(this));
    if (!strcmp(_clname, "idoc::ICommandsProvider"))
	return static_cast< idoc::ICommandsProvider*>(const_cast< CTextEditorComp*>(this));
    typedef iqt::TGuiObserverWrap<iqt::TGuiComponentBase<QTextEdit>,imod::TSingleModelObserverBase<IText> > QMocSuperClass;
    return QMocSuperClass::qt_metacast(_clname);
}

int CTextEditorComp::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    typedef iqt::TGuiObserverWrap<iqt::TGuiComponentBase<QTextEdit>,imod::TSingleModelObserverBase<IText> > QMocSuperClass;
    _id = QMocSuperClass::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: OnSelectionChanged(); break;
        case 1: OnTextChanged(); break;
        case 2: OnToLowercase(); break;
        case 3: OnToUppercase(); break;
        }
        _id -= 4;
    }
    return _id;
}
