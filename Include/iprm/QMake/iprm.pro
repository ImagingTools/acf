TARGET = iprm
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

HEADERS += ../CComposedParamsSetComp.h
HEADERS += ../CEnableableParamComp.h
HEADERS += ../CFileNameParamComp.h
HEADERS += ../CParamsManagerComp.h
HEADERS += ../CParamsSet.h
HEADERS += ../CSelectableParamsSetComp.h
HEADERS += ../IFileNameParam.h
HEADERS += ../IParamsManager.h
HEADERS += ../IParamsSet.h
HEADERS += ../iprm.h
HEADERS += ../ISelectionParam.h
SOURCES += ../CComposedParamsSetComp.cpp
SOURCES += ../CEnableableParamComp.cpp
SOURCES += ../CFileNameParamComp.cpp
SOURCES += ../CParamsManagerComp.cpp
SOURCES += ../CParamsSet.cpp
SOURCES += ../CSelectableParamsSetComp.cpp
