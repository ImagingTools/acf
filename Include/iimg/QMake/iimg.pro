TARGET = iimg
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

HEADERS += ../CBitmapBase.h
HEADERS += ../CGeneralBitmap.h
HEADERS += ../CHistogram.h
HEADERS += ../IBitmap.h
HEADERS += ../iimg.h
HEADERS += ../IRasterImage.h
HEADERS += ../TBitmapIterator.h
HEADERS += ../TBitmapIterator2d.h
HEADERS += ../TPixelConversion.h
SOURCES += ../CBitmapBase.cpp
SOURCES += ../CGeneralBitmap.cpp
SOURCES += ../CHistogram.cpp
