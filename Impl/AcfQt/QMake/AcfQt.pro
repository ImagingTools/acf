TARGET = AcfQt
TEMPLATE = lib
CONFIG += staticlib
CONFIG += lib_bundle
CONFIG += no_lflags_merge no_smart_library_merge

CONFIG(debug, debug|release) {
	DESTDIR = ../../../Lib/DebugQMake
        DEPENDPATH += ../../../Lib/DebugQMake
}
CONFIG(release, debug|release) {
	DESTDIR = ../../../Lib/ReleaseQMake
        DEPENDPATH += ../../../Lib/ReleaseQMake
}

OBJECTS += iqt.lib iqt2d.lib iqtdoc.lib iqtgui.lib iqtprm.lib iqtproc.lib iqtmm.lib

INCLUDEPATH += ../../ 

SOURCES += ../Empty.cpp
