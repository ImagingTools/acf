TARGET = isys
TEMPLATE = lib
CONFIG += staticlib

CONFIG(debug, debug|release){
	DESTDIR = ../../../Lib/DebugQMake
}
CONFIG(release, debug|release){
	DESTDIR = ../../../Lib/ReleaseQMake
}

QT += 

INCLUDEPATH += ../../ 

HEADERS += ../*.h
SOURCES += ../*.cpp
