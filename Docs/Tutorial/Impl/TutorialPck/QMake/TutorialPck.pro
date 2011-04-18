include(../../../../../Config/QMake/Component.config)
include(../../../../../Config/QMake/QtGuiBase.config)

TARGET = TutorialPck

DESTDIR = ../../../../../Bin/$$COMPILER_DIR

QT += main core gui xml sql network

INCLUDEPATH += ../..

HEADERS += ../TutorialPck.h
SOURCES += ../Main.cpp
