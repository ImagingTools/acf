include(../../AcfStd/QMake/AcfStd.pri)
include(../../AcfQt/QMake/AcfQt.pri)

TARGET = Acf
TEMPLATE = app
CONFIG += link_prl

CONFIG(debug, debug|release){
	DESTDIR = ../../../Bin/DebugQMake
	LIBS += -L../../../Lib/DebugQMake 
}
CONFIG(release, debug|release){
	DESTDIR = ../../../Bin/ReleaseQMake
	LIBS += -L../../../Lib/ReleaseQMake 
}

win32-msvc*{
	LIBS += -licmpstr -lAcfQt -lAcfStd
}
else{
	LIBS += -licmpstr $$ACFQT_LIBS $$ACFSTD_LIBS
}

UI_DIR = ../Generated
MOC_DIR = ../Generated
RCC_DIR = ../Generated

QT += core gui xml 


INCLUDEPATH += ../../../Include
INCLUDEPATH += ../../../Impl
INCLUDEPATH += "$(QTDIR)/include" "$(QTDIR)/include/QtCore" "$(QTDIR)/include/QtGui" "$(QTDIR)/include/QtXml" 

SOURCES += ../main.cpp
