TARGET = i2d
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

HEADERS += ../CAnnulus.h
HEADERS += ../CAnnulusSegment.h
HEADERS += ../CCircle.h
HEADERS += ../CLine2d.h
HEADERS += ../CPosition2d.h
HEADERS += ../CQuadrangle.h
HEADERS += ../CRectangle.h
HEADERS += ../CTransform.h
HEADERS += ../CVector2d.h
HEADERS += ../i2d.h
HEADERS += ../IObject2d.h
SOURCES += ../CAnnulus.cpp
SOURCES += ../CAnnulusSegment.cpp
SOURCES += ../CCircle.cpp
SOURCES += ../CLine2d.cpp
SOURCES += ../CPosition2d.cpp
SOURCES += ../CQuadrangle.cpp
SOURCES += ../CRectangle.cpp
SOURCES += ../CTransform.cpp
SOURCES += ../CVector2d.cpp
