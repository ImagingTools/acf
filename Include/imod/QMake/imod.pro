TARGET = imod
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

HEADERS += ../CModelBase.h
HEADERS += ../CModelChangeObserver.h
HEADERS += ../CModelProxy.h
HEADERS += ../CMultiModelBridgeBase.h
HEADERS += ../CMultiModelObserverBase.h
HEADERS += ../CSerializedUndoManager.h
HEADERS += ../CSingleModelObserverBase.h
HEADERS += ../imod.h
HEADERS += ../IModel.h
HEADERS += ../IModelEditor.h
HEADERS += ../IModelSelection.h
HEADERS += ../IObserver.h
HEADERS += ../IUndoManager.h
HEADERS += ../TModelWrap.h
HEADERS += ../TMultiModelObserverBase.h
HEADERS += ../TSingleModelObserverBase.h
SOURCES += ../CModelBase.cpp
SOURCES += ../CModelChangeObserver.cpp
SOURCES += ../CModelProxy.cpp
SOURCES += ../CMultiModelBridgeBase.cpp
SOURCES += ../CMultiModelObserverBase.cpp
SOURCES += ../CSerializedUndoManager.cpp
SOURCES += ../CSingleModelObserverBase.cpp
