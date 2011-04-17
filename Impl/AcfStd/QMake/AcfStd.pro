include(../../../Config/QMake/Static.config)
include(./AcfStd.pri)

TARGET = AcfStd

CONFIG += lib_bundle
CONFIG += no_lflags_merge no_smart_library_merge

DEPENDPATH += $$DESTDIR

LIBS += -L$$DESTDIR

OBJECTS += $$ACFSTD_DEPENDENCIES

INCLUDEPATH += ../../

SOURCES += ../Empty.cpp
