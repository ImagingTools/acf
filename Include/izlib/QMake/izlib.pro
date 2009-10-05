TARGET = izlib
TEMPLATE = lib
CONFIG += staticlib

CONFIG(debug, debug|release) {
	DESTDIR = ..\..\..\Lib\DebugQMake
}
CONFIG(release, debug|release) {
	DESTDIR = ..\..\..\Lib\ReleaseQMake
}

UI_DIR = ../Generated
MOC_DIR = ../Generated
RCC_DIR = ../Generated

QT += 


INCLUDEPATH += ../../ "$(ZLIBDIR)/include" 

HEADERS += ../CGzXmlReadArchive.h
HEADERS += ../CGzXmlWriteArchive.h
HEADERS += ../izlib.h
SOURCES += ../CGzXmlReadArchive.cpp
SOURCES += ../CGzXmlWriteArchive.cpp
