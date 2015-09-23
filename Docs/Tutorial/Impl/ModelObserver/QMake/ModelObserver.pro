TARGET = ModelObserver

include(../../../../..//Config/QMake/StaticConfig.pri)
include(../../../../..//Config/QMake/QtGuiBaseConfig.pri)

DESTDIR = $$OUT_PWD/../../../../../Lib/$$COMPILER_DIR/Tutorial

FORMS += $$_PRO_FILE_PWD_/../*.ui

INCLUDEPATH += ..
