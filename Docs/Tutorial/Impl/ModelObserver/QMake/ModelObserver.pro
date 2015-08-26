include(../../../../..//Config/QMake/StaticConfig.pri)
include(../../../../..//Config/QMake/QtGuiBaseConfig.pri)

TARGET = ModelObserver

DESTDIR = ../../../../../Lib/$$COMPILER_DIR

FORMS += $$_PRO_FILE_PWD_/../*.ui
UI_DIR = ../../GeneratedFiles/ModelObserver

INCLUDEPATH += ..
