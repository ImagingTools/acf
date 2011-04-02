TARGET = CompositorMini
TEMPLATE = app

CONFIG(debug, debug|release){
	DESTDIR = ../../../Bin/DebugQMake
	LIBS += -L../../../Lib/DebugQMake
}
CONFIG(release, debug|release){
    DESTDIR = ../../../Bin/ReleaseQMake
	LIBS += -L../../../Lib/ReleaseQMake
}

LIBS += -lAcfQt -lAcfStd -licmpstr

UI_DIR = ../Generated
MOC_DIR = ../Generated
RCC_DIR = ../Generated

QT += core gui

INCLUDEPATH = ../../../Include
INCLUDEPATH += "$(ACFDIR)/Include" "$(ACFDIR)/Impl" "$(QTDIR)/include" "$(QTDIR)/include/QtCore" "$(QTDIR)/include/QtGui" "$(QTDIR)/include/QtXml" "$(QTDIR)/include/QtScript" 

HEADERS += ../VC/resource.h
SOURCES += ../main.cpp
