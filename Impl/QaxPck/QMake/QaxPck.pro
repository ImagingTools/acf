TARGET = QaxPck
TEMPLATE = lib

CONFIG += dll

TARGET_EXT = arp

CONFIG(debug, debug|release) {
	DESTDIR = ..\..\..\Bin\DebugQMake
	LIBS += -L"$(ACFDIR)/Lib/DebugQMake -L"$(QScintilla)/Lib/DebugQMake -L"$(QWT3DDIR)/Lib/DebugQMake 
	LIBS += AcfStd AcfQt QAxContainerd 
}
CONFIG(release, debug|release) {
	DESTDIR = ..\..\..\Bin\ReleaseQMake
	LIBS += -L"$(ACFDIR)/Lib/ReleaseQMake -L"$(QScintilla)/Lib/ReleaseQMake -L"$(QWT3DDIR)/Lib/ReleaseQMake 
	LIBS += AcfStd AcfQt QAxContainer 
}

UI_DIR = ../Generated
MOC_DIR = ../Generated
RCC_DIR = ../Generated

QT += main core gui 


INCLUDEPATH += ../../../Include "$(QTDIR)/include" "$(QTDIR)/include/QtCore" "$(QTDIR)/include/QtGui" "$(QTDIR)/include/QtXml" "$(QTDIR)/include/QtScript" "$(QTDIR)/include/phonon" "$(QTDIR)/include/ActiveQt" 

HEADERS += ../QaxPck.h
SOURCES += ../QaxPck.cpp
