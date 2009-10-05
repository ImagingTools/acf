TARGET = icmpstr
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


INCLUDEPATH += ../../ "$(QTDIR)/include" "$(QTDIR)/include/QtCore" "$(QTDIR)/include/QtGui" "$(QTDIR)/include/QtXml" 

HEADERS += ../CAttributeEditorComp.h
HEADERS += ../CComponentConnector.h
HEADERS += ../CComponentHelpFileProviderComp.h
HEADERS += ../CComponentSceneItem.h
HEADERS += ../CConnectorShape.h
HEADERS += ../CGeometricalRegistryElement.h
HEADERS += ../CNote.h
HEADERS += ../CNoteSceneItem.h
HEADERS += ../CPackageOverviewComp.h
HEADERS += ../CRegistryElementShape.h
HEADERS += ../CRegistryLoaderComp.h
HEADERS += ../CRegistryModelComp.h
HEADERS += ../CRegistryPreviewComp.h
HEADERS += ../CRegistryView.h
HEADERS += ../CRegistryViewComp.h
HEADERS += ../IAttributeSelectionObserver.h
HEADERS += ../icmpstr.h
HEADERS += ../IComponentNote.h
HEADERS += ../IElementSelectionInfo.h
HEADERS += ../IRegistryEditController.h
HEADERS += ../IRegistryPreview.h
SOURCES += ../CAttributeEditorComp.cpp
SOURCES += ../CComponentConnector.cpp
SOURCES += ../CComponentHelpFileProviderComp.cpp
SOURCES += ../CComponentSceneItem.cpp
SOURCES += ../CConnectorShape.cpp
SOURCES += ../CGeometricalRegistryElement.cpp
SOURCES += ../CNote.cpp
SOURCES += ../CNoteSceneItem.cpp
SOURCES += ../CPackageOverviewComp.cpp
SOURCES += ../CRegistryElementShape.cpp
SOURCES += ../CRegistryLoaderComp.cpp
SOURCES += ../CRegistryModelComp.cpp
SOURCES += ../CRegistryPreviewComp.cpp
SOURCES += ../CRegistryView.cpp
SOURCES += ../CRegistryViewComp.cpp
FORMS += ../CAttributeEditorComp.ui
FORMS += ../CPackageOverviewComp.ui
FORMS += ../CRegistryPropertiesDialog.ui
RESOURCES += ../icmpstr.qrc
