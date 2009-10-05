TARGET = iqsci
TEMPLATE = lib
CONFIG += staticlib

CONFIG(debug, debug|release) {
	DESTDIR = ..\..\..\Lib\DebugQMake
}
CONFIG(release, debug|release) {
	DESTDIR = ..\..\..\Lib\ReleaseQMake
}

UI_DIR = ../Generated
MOC_DIR = ../Generated
RCC_DIR = ../Generated

QT += 


INCLUDEPATH += ../../ "$(QTDIR)/include" "$(QTDIR)/include/QtCore" "$(QTDIR)/include/QtGui" "$(QTDIR)/include/QtXml" "$(QScintilla)/include" 

HEADERS += ../CTextEditor.h
HEADERS += ../CTextEditorGuiComp.h
HEADERS += ../iqsci.h
SOURCES += ../CTextEditor.cpp
SOURCES += ../CTextEditorGuiComp.cpp
FORMS += ../CTextEditor.ui
