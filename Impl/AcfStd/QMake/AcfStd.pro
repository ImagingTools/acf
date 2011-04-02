TARGET = AcfStd
TEMPLATE = lib
CONFIG += staticlib
CONFIG += lib_bundle
CONFIG += no_lflags_merge no_smart_library_merge

CONFIG(debug, debug|release){
        DESTDIR = ../../../Lib/DebugQMake
        DEPENDPATH += ../../../Lib/DebugQMake
}
CONFIG(release, debug|release){
        DESTDIR = ../../../Lib/ReleaseQMake
        DEPENDPATH += ../../../Lib/ReleaseQMake
}


# static libs for visual studio compiler
MSVC_OBJECTS = i2d.lib ibase.lib icmm.lib icomp.lib idoc.lib iimg.lib imath.lib imod.lib iprm.lib isec.lib iser.lib istd.lib isys.lib iproc.lib

# static libs for the GCC compiler
GCC_OBJECTS = libibase.a libicomp.a libi2d.a libiproc.a libicmm.a libidoc.a libiimg.a libimath.a libimod.a libiprm.a libisec.a libiser.a libisys.a libistd.a

win32-msvc*{
	OBJECTS = $$MSVC_OBJECTS
}
else{
	OBJECTS = $$GCC_OBJECTS
}

INCLUDEPATH += ../../

SOURCES += ../Empty.cpp
