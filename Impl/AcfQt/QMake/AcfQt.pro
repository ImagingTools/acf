include(../../../Config/QMake/Static.config)
include(./AcfQt.pri)

TARGET = AcfQt

CONFIG += lib_bundle
CONFIG += no_lflags_merge no_smart_library_merge

DEPENDPATH += $$DESTDIR

LIBS += -L$$DESTDIR

OBJECTS += $$ACFQT_DEPENDENCIES

INCLUDEPATH += ../../ 

SOURCES += ../Empty.cpp
