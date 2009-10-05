TARGET = isys
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

HEADERS += ../CDateTimeBase.h
HEADERS += ../CSectionBlocker.h
HEADERS += ../CSimpleDateTime.h
HEADERS += ../CSystemEnvironmentBase.h
HEADERS += ../CSystemInfoBase.h
HEADERS += ../IApplicationEnvironment.h
HEADERS += ../ICriticalSection.h
HEADERS += ../IDateTime.h
HEADERS += ../IFileSystem.h
HEADERS += ../IFolderMonitor.h
HEADERS += ../IFunctionsProvider.h
HEADERS += ../isys.h
HEADERS += ../ISystemEnvironment.h
HEADERS += ../ISystemInfo.h
HEADERS += ../ITimer.h
SOURCES += ../CDateTimeBase.cpp
SOURCES += ../CSimpleDateTime.cpp
SOURCES += ../CSystemEnvironmentBase.cpp
SOURCES += ../CSystemInfoBase.cpp
