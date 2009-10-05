TARGET = XlnPck
TEMPLATE = lib

CONFIG += dll

TARGET_EXT = arp

CONFIG(debug, debug|release) {
	DESTDIR = ..\..\..\Bin\DebugQMake
	LIBS += -L"$(ACFDIR)/Lib/DebugQMake -L"$(XERCESDIR)/lib" -L"$(XALANDIR)/lib" 
	LIBS += AcfStd ixln xerces-c_2D Xalan-C_1D 
}
CONFIG(release, debug|release) {
	DESTDIR = ..\..\..\Bin\ReleaseQMake
	LIBS += -L"$(ACFDIR)/Lib/ReleaseQMake -L"$(XERCESDIR)/lib" -L"$(XALANDIR)/lib" 
	LIBS += AcfStd ixln xerces-c_2 Xalan-C_1 
}

UI_DIR = ../Generated
MOC_DIR = ../Generated
RCC_DIR = ../Generated

QT += 


INCLUDEPATH += ../../../Include "$(XERCESDIR)/src" "$(XALANDIR)/src" 

HEADERS += ../XlnPck.h
SOURCES += ../XlnPck.cpp
