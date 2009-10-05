TARGET = ibase
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

HEADERS += ../CApplicationInfoComp.h
HEADERS += ../CComposedLoaderComp.h
HEADERS += ../CConsoleLogComp.h
HEADERS += ../CCopyAppComp.h
HEADERS += ../CFileCopyOverLoaderComp.h
HEADERS += ../CFilePersistenceComp.h
HEADERS += ../CFileSystem.h
HEADERS += ../CInstantiatorComp.h
HEADERS += ../CMessage.h
HEADERS += ../CModelBinderComp.h
HEADERS += ../CModelProxyComp.h
HEADERS += ../CRectangleComp.h
HEADERS += ../CRegistryCodeSaverComp.h
HEADERS += ../CSize.h
HEADERS += ../CStreamLogCompBase.h
HEADERS += ../CTextDocumentComp.h
HEADERS += ../CTextFileLoaderComp.h
HEADERS += ../CTextFileLogComp.h
HEADERS += ../IApplication.h
HEADERS += ../IApplicationInfo.h
HEADERS += ../ibase.h
HEADERS += ../IFileConvertCopy.h
HEADERS += ../ILoggable.h
HEADERS += ../IMessage.h
HEADERS += ../IMessageConsumer.h
HEADERS += ../IMessageContainer.h
HEADERS += ../INamedObject.h
HEADERS += ../IObject.h
HEADERS += ../ITextDocument.h
HEADERS += ../TComposedFactoryComp.h
HEADERS += ../TContainer.h
HEADERS += ../TEnableableWrap.h
HEADERS += ../TFactorisableContainer.h
HEADERS += ../TFileSerializerComp.h
HEADERS += ../THierarchicalBase.h
HEADERS += ../TLoggerCompWrap.h
HEADERS += ../TLoggerWrap.h
HEADERS += ../TMessageContainerWrap.h
HEADERS += ../TMsbWord.h
HEADERS += ../TNamedWrap.h
HEADERS += ../TSerializableContainer.h
HEADERS += ../TSingleFactoryComp.h
SOURCES += ../CApplicationInfoComp.cpp
SOURCES += ../CComposedLoaderComp.cpp
SOURCES += ../CConsoleLogComp.cpp
SOURCES += ../CCopyAppComp.cpp
SOURCES += ../CFileCopyOverLoaderComp.cpp
SOURCES += ../CFilePersistenceComp.cpp
SOURCES += ../CFileSystem.cpp
SOURCES += ../CInstantiatorComp.cpp
SOURCES += ../CMessage.cpp
SOURCES += ../CModelBinderComp.cpp
SOURCES += ../CModelProxyComp.cpp
SOURCES += ../CRectangleComp.cpp
SOURCES += ../CRegistryCodeSaverComp.cpp
SOURCES += ../CSize.cpp
SOURCES += ../CStreamLogCompBase.cpp
SOURCES += ../CTextDocumentComp.cpp
SOURCES += ../CTextFileLoaderComp.cpp
SOURCES += ../CTextFileLogComp.cpp
