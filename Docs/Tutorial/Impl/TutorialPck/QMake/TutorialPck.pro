include(../../../../../Config/QMake/ComponentConfig.pri)
include(../../../../../Config/QMake/QtGuiBaseConfig.pri)

TARGET = TutorialPck

DESTDIR = ../../../../../Bin/$$COMPILER_DIR

QT += main core gui xml sql network

INCLUDEPATH += ../..
