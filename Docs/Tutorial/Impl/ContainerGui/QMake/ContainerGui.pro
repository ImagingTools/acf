include(../../../../../Config/QMake/ApplicationConfig.pri)
include(../../../../../Config/QMake/QtGuiBaseConfig.pri)

TARGET = ContainerGui

DESTDIR = ../../../../../Bin/$$COMPILER_DIR

FORMS += $$_PRO_FILE_PWD_/../*.ui
UI_DIR = ../../GeneratedFiles/ContainerGui

LIBS += -lAcfStd -lAcfQt

QT += main core gui 


