TARGET = CompositorMini
TEMPLATE = app

CONFIG(debug, debug|release) {
	DESTDIR = ..\..\..\Bin\DebugQMake
	LIBS += -L"$(ACFDIR)/Lib/DebugQMake -L"$(QScintilla)/Lib/DebugQMake 
	LIBS += 
}
CONFIG(release, debug|release) {
	DESTDIR = ..\..\..\Bin\ReleaseQMake
	LIBS += -L"$(ACFDIR)/Lib/ReleaseQMake -L"$(QScintilla)/Lib/ReleaseQMake 
	LIBS += 
}

UI_DIR = ../Generated
MOC_DIR = ../Generated
RCC_DIR = ../Generated

QT += main core gui 

include(dependencies.pri)

INCLUDEPATH += "$(ACFDIR)/Include" "$(ACFDIR)/Impl" "$(QTDIR)/include" "$(QTDIR)/include/QtCore" "$(QTDIR)/include/QtGui" "$(QTDIR)/include/QtXml" "$(QTDIR)/include/QtScript" 

SOURCES += ../main.cpp
