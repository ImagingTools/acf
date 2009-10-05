TARGET = inat
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

HEADERS += ../CCriticalSection.h
HEADERS += ../CDllFunctionsProvider.h
HEADERS += ../CTimer.h
HEADERS += ../inat.h
HEADERS += ../TComPtr.h
