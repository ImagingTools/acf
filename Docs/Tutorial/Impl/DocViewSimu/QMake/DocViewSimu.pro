include(../../../../../Config/QMake/ApplicationConfig.pri)
include(../../../../../Config/QMake/QtGuiBaseConfig.pri)

TARGET = DocViewSimu

DESTDIR = ../../../../../Bin/$$COMPILER_DIR/Tutorial

LIBS += -lAcfStd -lAcfQt

FORMS += $$_PRO_FILE_PWD_/../*.ui

QT += main core gui


INCLUDEPATH += ../.. ../../../../../Impl "$(QScintilla)/include"

