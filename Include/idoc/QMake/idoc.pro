TARGET = idoc
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

HEADERS += ../CCompositeDocumentTemplateComp.h
HEADERS += ../CDocumentManagerBase.h
HEADERS += ../CDocumentManagerListenerComp.h
HEADERS += ../CMultiDocumentManagerBase.h
HEADERS += ../CSerializedStateComparator.h
HEADERS += ../CSingleDocumentManagerBase.h
HEADERS += ../CSingleDocumentTemplateBase.h
HEADERS += ../CSingleDocumentTemplateComp.h
HEADERS += ../ICommand.h
HEADERS += ../ICommandsProvider.h
HEADERS += ../idoc.h
HEADERS += ../IDocumentManager.h
HEADERS += ../IDocumentStateComparator.h
HEADERS += ../IDocumentTemplate.h
HEADERS += ../IHelpFileProvider.h
HEADERS += ../IHelpViewer.h
HEADERS += ../IMainWindowCommands.h
SOURCES += ../CCompositeDocumentTemplateComp.cpp
SOURCES += ../CDocumentManagerBase.cpp
SOURCES += ../CDocumentManagerListenerComp.cpp
SOURCES += ../CMultiDocumentManagerBase.cpp
SOURCES += ../CSerializedStateComparator.cpp
SOURCES += ../CSingleDocumentManagerBase.cpp
SOURCES += ../CSingleDocumentTemplateBase.cpp
SOURCES += ../CSingleDocumentTemplateComp.cpp
