TARGET = AcfQt
TEMPLATE = lib
CONFIG += staticlib
CONFIG += lib_bundle
CONFIG += no_lflags_merge no_smart_library_merge
CONFIG += create_prl link_prl

CONFIG(debug, debug|release){
	DESTDIR = ../../../Lib/DebugQMake
        DEPENDPATH += ../../../Lib/DebugQMake
}
CONFIG(release, debug|release){
	DESTDIR = ../../../Lib/ReleaseQMake
        DEPENDPATH += ../../../Lib/ReleaseQMake
}

# static libs for visual studio compiler
MSVC_OBJECTS = iqt.lib iqt2d.lib iqtdoc.lib iqtgui.lib iqtprm.lib iqtproc.lib iqtmm.lib

# static libs for the GCC compiler
GCC_OBJECTS = libiqtmm.a libiqtproc.a libiqt2d.a libiqtdoc.a libiqtprm.a libiqtgui.a libiqt.a

win32{
	win32-g++{
		OBJECTS = $$GCC_OBJECTS
	}
	else{
		OBJECTS = $$MSVC_OBJECTS
	}
}
else{
	OBJECTS = $$GCC_OBJECTS
}

INCLUDEPATH += ../../ 

SOURCES += ../Empty.cpp
