TARGET = AcfStd
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

OBJECTS += i2d.lib ibase.lib icmm.lib icomp.lib idoc.lib iimg.lib imath.lib imod.lib iprm.lib isec.lib iser.lib istd.lib isys.lib iproc.lib

INCLUDEPATH += ../../ 

SOURCES += ../Empty.cpp
