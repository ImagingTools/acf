TARGET = DocView
TEMPLATE = lib
CONFIG += staticlib

CONFIG(debug, debug|release){
	DESTDIR = ../../../../../Lib/DebugQMake
}
CONFIG(release, debug|release){
	DESTDIR = ../../../../../Lib/ReleaseQMake
}

UI_DIR = ../Generated
MOC_DIR = ../Generated
RCC_DIR = ../Generated

QT += 


INCLUDEPATH += .. ../../../../../Include "$(QTDIR)/Include" "$(QTDIR)/Include/QtCore" "$(QTDIR)/Include/QtGui" "$(ACFDIR)/Include" 

HEADERS += ../CTextEditorComp.h
SOURCES += ../CTextEditorComp.cpp
