TARGET = iqtprm
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

HEADERS += ../CComposedParamsSetGuiComp.h
HEADERS += ../CFileNameParamGuiComp.h
HEADERS += ../CParamsManagerGuiComp.h
HEADERS += ../iqtprm.h
SOURCES += ../CComposedParamsSetGuiComp.cpp
SOURCES += ../CFileNameParamGuiComp.cpp
SOURCES += ../CParamsManagerGuiComp.cpp
FORMS += ../CComposedParamsSetGuiComp.ui
FORMS += ../CFileNameParamGuiComp.ui
FORMS += ../CParamsManagerGuiComp.ui
