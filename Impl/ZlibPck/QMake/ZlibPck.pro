TARGET = ZlibPck
TEMPLATE = lib

CONFIG += dll

TARGET_EXT = arp

CONFIG(debug, debug|release) {
	DESTDIR = ..\..\..\Bin\DebugQMake
	LIBS += -L"$(ACFDIR)/Lib/DebugQMake -L"$(ZLIBDIR)/lib" -L"$(INTELCDIR)/lib" 
	LIBS += AcfStd izlib d_zlib 
}
CONFIG(release, debug|release) {
	DESTDIR = ..\..\..\Bin\ReleaseQMake
	LIBS += -L"$(ACFDIR)/Lib/ReleaseQMake -L"$(ZLIBDIR)/lib" -L"$(INTELCDIR)/lib" 
	LIBS += AcfStd izlib zlib 
}

UI_DIR = ../Generated
MOC_DIR = ../Generated
RCC_DIR = ../Generated

QT += 


INCLUDEPATH += ../../../Include "$(ZLIBDIR)/include" 

HEADERS += ../ZlibPck.h
SOURCES += ../ZlibPck.cpp
