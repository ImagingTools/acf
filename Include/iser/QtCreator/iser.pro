# -------------------------------------------------
# Project created by QtCreator 2008-11-24T15:40:24
# -------------------------------------------------
QT -= core \
    gui
TARGET = iser
TEMPLATE = lib
CONFIG += staticlib
INCLUDEPATH+=$(ACFDIR)/include
SOURCES += ../CXmlWriteArchiveBase.cpp \
    ../CXmlStringWriteArchive.cpp \
    ../CXmlStringReadArchive.cpp \
    ../CXmlReadArchiveBase.cpp \
    ../CXmlFileWriteArchive.cpp \
    ../CXmlFileReadArchive.cpp \
    ../CXmlDocumentInfoBase.cpp \
    ../CWriteArchiveBase.cpp \
    ../CTextWriteArchiveBase.cpp \
    ../CTextReadArchiveBase.cpp \
    ../CReaderVersionInfo.cpp \
    ../CReadArchiveBase.cpp \
    ../CMinimalVersionInfo.cpp \
    ../CMemoryWriteArchive.cpp \
    ../CMemoryReadArchive.cpp \
    ../CFileWriteArchive.cpp \
    ../CFileReadArchive.cpp \
    ../CBitMemoryWriteArchive.cpp \
    ../CBitMemoryReadArchive.cpp \
    ../CBinaryWriteArchiveBase.cpp \
    ../CBinaryReadArchiveBase.cpp \
    ../CArchiveTag.cpp \
    ../CArchiveBase.cpp
HEADERS += ../TXmlStreamReadArchiveBase.h \
    ../IVersionInfo.h \
    ../ISerializable.h \
    ../iser.h \
    ../IFileLoaderInfo.h \
    ../IFileLoader.h \
    ../IArchive.h \
    ../CXmlWriteArchiveBase.h \
    ../CXmlStringWriteArchive.h \
    ../CXmlStringReadArchive.h \
    ../CXmlReadArchiveBase.h \
    ../CXmlFileWriteArchive.h \
    ../CXmlFileReadArchive.h \
    ../CXmlDocumentInfoBase.h \
    ../CWriteArchiveBase.h \
    ../CTextWriteArchiveBase.h \
    ../CTextReadArchiveBase.h \
    ../CReaderVersionInfo.h \
    ../CReadArchiveBase.h \
    ../CMinimalVersionInfo.h \
    ../CMemoryWriteArchive.h \
    ../CMemoryReadArchive.h \
    ../CFileWriteArchive.h \
    ../CFileReadArchive.h \
    ../CBitMemoryWriteArchive.h \
    ../CBitMemoryReadArchive.h \
    ../CBinaryWriteArchiveBase.h \
    ../CBinaryReadArchiveBase.h \
    ../CArchiveTag.h \
    ../CArchiveBase.h \
    ../TXmlStreamWriteArchiveBase.h
