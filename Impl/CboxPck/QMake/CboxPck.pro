TARGET = CboxPck
TEMPLATE = lib

CONFIG += dll

TARGET_EXT = arp

CONFIG(debug, debug|release) {
	DESTDIR = ..\..\..\Bin\DebugQMake
	LIBS += -L"$(ACFDIR)/Lib/DebugQMake -L"$(CBIOSDIR)/lib/QMake" 
	LIBS += cbios wsock32 
}
CONFIG(release, debug|release) {
	DESTDIR = ..\..\..\Bin\ReleaseQMake
	LIBS += -L"$(ACFDIR)/Lib/ReleaseQMake -L"$(CBIOSDIR)/lib/QMake" 
	LIBS += cbios wsock32 
}

UI_DIR = ../Generated
MOC_DIR = ../Generated
RCC_DIR = ../Generated

QT += 


INCLUDEPATH += ../../../Include "$(CBIOSDIR)/include" 

HEADERS += ../CboxPck.h
SOURCES += ../CboxPck.cpp
