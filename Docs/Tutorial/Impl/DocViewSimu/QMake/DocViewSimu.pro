include(../../../../../Config/QMake/Application.config)
include(../../../../../Config/QMake/QtGuiBase.config)

TARGET = DocViewSimu

DESTDIR = ../../../../../Bin/$$COMPILER_DIR

LIBS += -lAcfStd -lAcfQt

UI_DIR = ../Generated
MOC_DIR = ../Generated
RCC_DIR = ../Generated

QT += main core gui 


INCLUDEPATH += ../.. ../../../../../Impl "$(QScintilla)/include"

SOURCES += ../*.cpp
