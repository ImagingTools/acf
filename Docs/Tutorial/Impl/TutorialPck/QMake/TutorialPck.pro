TARGET = TutorialPck

include(../../../../../Config/QMake/ComponentConfig.pri)
include(../../../../../Config/QMake/QtGuiBaseConfig.pri)

DESTDIR = $$OUT_PWD/../../../../../Bin/$$COMPILER_DIR/Tutorial

QT += core gui xml sql network

FORMS += $$_PRO_FILE_PWD_/../*.ui
UI_DIR = ../../GeneratedFiles/TutorialPck

INCLUDEPATH += ../.. ../../../Include

LIBS += -L$$OUT_PWD/../../../../../Lib/$$COMPILER_DIR/Tutorial -litutacf -lDocView -lModelObserver

include(../../../../../Config/QMake/AcfQt.pri)
include(../../../../../Config/QMake/AcfStd.pri)
