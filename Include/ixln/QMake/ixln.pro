TARGET = ixln
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


INCLUDEPATH += ../../ "$(XERCESDIR)/src" "$(XALANDIR)/src" 

HEADERS += ../CFileXmlReadArchive.h
HEADERS += ../CFileXmlWriteArchive.h
HEADERS += ../CXercesBase.h
HEADERS += ../CXmlReadArchiveBase.h
HEADERS += ../CXmlWriteArchiveBase.h
HEADERS += ../ixln.h
SOURCES += ../CFileXmlReadArchive.cpp
SOURCES += ../CFileXmlWriteArchive.cpp
SOURCES += ../CXercesBase.cpp
SOURCES += ../CXmlReadArchiveBase.cpp
SOURCES += ../CXmlWriteArchiveBase.cpp
