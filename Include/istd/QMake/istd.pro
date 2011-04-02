TARGET = istd
TEMPLATE = lib
CONFIG += staticlib
CONFIG += stl

CONFIG(debug, debug|release){
        DESTDIR = ../../../Lib/DebugQMake
}
CONFIG(release, debug|release){
        DESTDIR = ../../../Lib/ReleaseQMake
}

INCLUDEPATH += ../../ 


win32:UpdateSvnVersion.commands = "UpdateVersion.bat"
QMAKE_EXTRA_TARGETS += UpdateSvnVersion

UpdateSvnVersionHook.depends = UpdateSvnVersion
CONFIG(debug,debug|release):UpdateSvnVersionHook.target = Makefile.Debug
CONFIG(release,debug|release):UpdateSvnVersionHook.target = Makefile.Release
QMAKE_EXTRA_TARGETS += UpdateSvnVersionHook

HEADERS += ../*.h
HEADERS += ../Generated/AcfVersion.h
SOURCES += ../*.cpp
