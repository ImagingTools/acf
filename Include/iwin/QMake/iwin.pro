TARGET = iwin
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

HEADERS += ../CApplicationEnvironment.h
HEADERS += ../CCriticalSection.h
HEADERS += ../CDefaultServicesProvider.h
HEADERS += ../CDllFunctionsProvider.h
HEADERS += ../CSystemEnvironment.h
HEADERS += ../CSystemInfo.h
HEADERS += ../CTimer.h
HEADERS += ../iwin.h
HEADERS += ../TComPtr.h
SOURCES += ../CApplicationEnvironment.cpp
SOURCES += ../CCriticalSection.cpp
SOURCES += ../CDefaultServicesProvider.cpp
SOURCES += ../CDllFunctionsProvider.cpp
SOURCES += ../CSystemEnvironment.cpp
SOURCES += ../CSystemInfo.cpp
SOURCES += ../CTimer.cpp
