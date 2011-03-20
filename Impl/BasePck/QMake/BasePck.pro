TARGET = BasePck
TEMPLATE = lib

CONFIG += dll

TARGET_EXT = .arp

CONFIG(debug, debug|release) {
        DESTDIR = ../../../Bin/DebugQMake
	LIBS += -L"$(ACFDIR)/Lib/DebugQMake 
        LIBS += AcfStd.lib
}
CONFIG(release, debug|release) {
        DESTDIR = ../../../Bin/ReleaseQMake
	LIBS += -L"$(ACFDIR)/Lib/ReleaseQMake 
        LIBS += AcfStd.lib
}

UI_DIR = ../Generated
MOC_DIR = ../Generated
RCC_DIR = ../Generated

INCLUDEPATH += ../../../Include "$(QTDIR)/include" "$(QTDIR)/include/QtCore" "$(QTDIR)/include/QtGui" "$(QTDIR)/include/QtXml" 

HEADERS += ../BasePck.h
SOURCES += ../BasePck.cpp
