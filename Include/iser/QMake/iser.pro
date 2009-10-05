TARGET = iser
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


INCLUDEPATH += ../../ 

HEADERS += ../CArchiveBase.h
HEADERS += ../CArchiveTag.h
HEADERS += ../CBinaryReadArchiveBase.h
HEADERS += ../CBinaryWriteArchiveBase.h
HEADERS += ../CBitMemoryReadArchive.h
HEADERS += ../CBitMemoryWriteArchive.h
HEADERS += ../CFileReadArchive.h
HEADERS += ../CFileWriteArchive.h
HEADERS += ../CMemoryReadArchive.h
HEADERS += ../CMemoryWriteArchive.h
HEADERS += ../CMinimalVersionInfo.h
HEADERS += ../CReadArchiveBase.h
HEADERS += ../CReaderVersionInfo.h
HEADERS += ../CTextReadArchiveBase.h
HEADERS += ../CTextWriteArchiveBase.h
HEADERS += ../CWriteArchiveBase.h
HEADERS += ../CXmlDocumentInfoBase.h
HEADERS += ../CXmlFileReadArchive.h
HEADERS += ../CXmlFileWriteArchive.h
HEADERS += ../CXmlReadArchiveBase.h
HEADERS += ../CXmlStringReadArchive.h
HEADERS += ../CXmlStringWriteArchive.h
HEADERS += ../CXmlWriteArchiveBase.h
HEADERS += ../IArchive.h
HEADERS += ../IFileLoader.h
HEADERS += ../IFileLoaderInfo.h
HEADERS += ../iser.h
HEADERS += ../ISerializable.h
HEADERS += ../IVersionInfo.h
HEADERS += ../TXmlStreamReadArchiveBase.h
HEADERS += ../TXmlStreamWriteArchiveBase.h
SOURCES += ../CArchiveBase.cpp
SOURCES += ../CArchiveTag.cpp
SOURCES += ../CBinaryReadArchiveBase.cpp
SOURCES += ../CBinaryWriteArchiveBase.cpp
SOURCES += ../CBitMemoryReadArchive.cpp
SOURCES += ../CBitMemoryWriteArchive.cpp
SOURCES += ../CFileReadArchive.cpp
SOURCES += ../CFileWriteArchive.cpp
SOURCES += ../CMemoryReadArchive.cpp
SOURCES += ../CMemoryWriteArchive.cpp
SOURCES += ../CMinimalVersionInfo.cpp
SOURCES += ../CReadArchiveBase.cpp
SOURCES += ../CReaderVersionInfo.cpp
SOURCES += ../CTextReadArchiveBase.cpp
SOURCES += ../CTextWriteArchiveBase.cpp
SOURCES += ../CWriteArchiveBase.cpp
SOURCES += ../CXmlDocumentInfoBase.cpp
SOURCES += ../CXmlFileReadArchive.cpp
SOURCES += ../CXmlFileWriteArchive.cpp
SOURCES += ../CXmlReadArchiveBase.cpp
SOURCES += ../CXmlStringReadArchive.cpp
SOURCES += ../CXmlStringWriteArchive.cpp
SOURCES += ../CXmlWriteArchiveBase.cpp
