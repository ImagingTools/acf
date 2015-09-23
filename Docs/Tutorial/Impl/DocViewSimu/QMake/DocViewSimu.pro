TARGET = DocViewSimu

include(../../../../../Config/QMake/ApplicationConfig.pri)
include(../../../../../Config/QMake/QtGuiBaseConfig.pri)

DESTDIR = $$OUT_PWD/../../../../../Bin/$$COMPILER_DIR/Tutorial

FORMS += $$_PRO_FILE_PWD_/../*.ui

QT += core gui

INCLUDEPATH += ../.. ../../../Include "$(QScintilla)/include"

LIBS += -L$$OUT_PWD/../../../../../Lib/$$COMPILER_DIR/Tutorial -lDocView

include(../../../../../Config/QMake/AcfQt.pri)
include(../../../../../Config/QMake/AcfStd.pri)
