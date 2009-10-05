TARGET = AbcPck
TEMPLATE = lib

CONFIG += dll

TARGET_EXT = arp

CONFIG(debug, debug|release) {
	DESTDIR = ..\..\..\Bin\DebugQMake
	LIBS += -L"$(ACFDIR)/Lib/DebugQMake -L"$(QScintilla)/Lib/DebugQMake -L"$(QWT3DDIR)/Lib/DebugQMake -L"$(QTSERVICEDIR)/Lib/DebugQMake 
	LIBS += qwt3dlib AcfStd AcfQt 
}
CONFIG(release, debug|release) {
	DESTDIR = ..\..\..\Bin\ReleaseQMake
	LIBS += -L"$(ACFDIR)/Lib/ReleaseQMake -L"$(QScintilla)/Lib/ReleaseQMake -L"$(QWT3DDIR)/Lib/ReleaseQMake -L"$(QTSERVICEDIR)/Lib/ReleaseQMake 
	LIBS += qwt3dlib AcfStd AcfQt 
}

UI_DIR = ../Generated
MOC_DIR = ../Generated
RCC_DIR = ../Generated

QT += main core gui xml sql network script service 


INCLUDEPATH += ../../../Include "$(QTDIR)/include" "$(QTDIR)/include/QtCore" "$(QTDIR)/include/QtGui" "$(QTDIR)/include/QtXml" "$(QTDIR)/include/QtScript" "$(QTSERVICEDIR)/Src" 

HEADERS += ../AbcPck.h
SOURCES += ../AbcPck.cpp
