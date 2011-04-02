TARGET = BasePck
TEMPLATE = lib

CONFIG += dll

TARGET_EXT = .arp

CONFIG(debug, debug|release){
	DESTDIR = ../../../Bin/DebugQMake
	LIBS += -L../../../Lib/DebugQMake 
}
CONFIG(release, debug|release){
	DESTDIR = ../../../Bin/ReleaseQMake
	LIBS += -L../../../Lib/ReleaseQMake 
}

LIBS += -lAcfStd

QT -= gui core

INCLUDEPATH += ../../../Include
HEADERS += ../BasePck.h
SOURCES += ../BasePck.cpp
