TARGET = idoc
TEMPLATE = lib
CONFIG += staticlib
CONFIG += stl

CONFIG(debug, debug|release) {
	DESTDIR = ../../../Lib/DebugQMake
}
CONFIG(release, debug|release) {
	DESTDIR = ../../../Lib/ReleaseQMake
}

INCLUDEPATH += ../../ 

HEADERS += ../*.h
SOURCES += ../*.cpp
