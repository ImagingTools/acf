TARGET = icmm
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

HEADERS += ../CCmy.h
HEADERS += ../CCmyk.h
HEADERS += ../CCmykToRgbTransformation.h
HEADERS += ../CColorTransformationProvider.h
HEADERS += ../CHsv.h
HEADERS += ../CHsvToRgbTransformation.h
HEADERS += ../CLab.h
HEADERS += ../CRgb.h
HEADERS += ../CRgbToCmykTransformation.h
HEADERS += ../CVarColor.h
HEADERS += ../icmm.h
HEADERS += ../IColorTransformation.h
HEADERS += ../TColorGradient.h
HEADERS += ../TComposedColor.h
HEADERS += ../TComposedColorGradient.h
SOURCES += ../CCmy.cpp
SOURCES += ../CCmyk.cpp
SOURCES += ../CCmykToRgbTransformation.cpp
SOURCES += ../CColorTransformationProvider.cpp
SOURCES += ../CHsv.cpp
SOURCES += ../CHsvToRgbTransformation.cpp
SOURCES += ../CLab.cpp
SOURCES += ../CRgb.cpp
SOURCES += ../CRgbToCmykTransformation.cpp
SOURCES += ../CVarColor.cpp
