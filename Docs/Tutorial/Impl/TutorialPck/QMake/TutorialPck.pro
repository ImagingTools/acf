TARGET = TutorialPck
TEMPLATE = lib

CONFIG += dll

TARGET_EXT = arp

CONFIG(debug, debug|release){
	DESTDIR = ../../../../../Bin/DebugQMake
	LIBS += -L"$(ACFDIR)/Lib/DebugQMake 
	LIBS += AcfStd AcfQt 
}
CONFIG(release, debug|release){
	DESTDIR = ../../../../../Bin/ReleaseQMake
	LIBS += -L"$(ACFDIR)/Lib/ReleaseQMake 
	LIBS += AcfStd AcfQt 
}

UI_DIR = ../Generated
MOC_DIR = ../Generated
RCC_DIR = ../Generated

QT += main core gui xml sql network 


INCLUDEPATH += ../.. ../../../../../Include "$(QTDIR)/include" "$(QTDIR)/include/QtCore" "$(QTDIR)/include/QtGui" "$(QTDIR)/include/QtXml" 

HEADERS += ../TutorialPck.h
SOURCES += ../Main.cpp
