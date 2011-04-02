TARGET = DocViewSimu_exe
TEMPLATE = app

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

QT += main core gui 


INCLUDEPATH += .. "$(QTDIR)/Include" "$(QTDIR)/Include/QtCore" "$(QTDIR)/Include/QtGui" "$(QTDIR)/Include/QtScript" "$(ACFDIR)/Include" "$(ACFDIR)/Impl" ../.. "$(QScintilla)/include" 

SOURCES += ../Main.cpp
