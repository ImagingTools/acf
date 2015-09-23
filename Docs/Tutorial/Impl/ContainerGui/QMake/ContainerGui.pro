TARGET = ContainerGui

include(../../../../../Config/QMake/ApplicationConfig.pri)
include(../../../../../Config/QMake/QtGuiBaseConfig.pri)

DESTDIR = $$OUT_PWD/../../../../../Bin/$$COMPILER_DIR/Tutorial

FORMS += $$_PRO_FILE_PWD_/../*.ui
UI_DIR = ../../GeneratedFiles/ContainerGui

QT += core gui 

include(../../../../../Config/QMake/AcfQt.pri)
include(../../../../../Config/QMake/AcfStd.pri)
