include(../../../Config/QMake/General.config)
include(../../../Config/QMake/QtBase.config)
include(../../AcfStd/QMake/AcfStd.pri)
include(../../AcfQt/QMake/AcfQt.pri)

TARGET = CompositorMini
TEMPLATE = app
CONFIG += link_prl

DESTDIR = ../../../Bin/$$COMPILER_DIR
LIBS += -L../../../Lib/$$COMPILER_DIR

LIBS += -licmpstr

INCLUDEPATH += ../../../Include ../../../Impl

SOURCES += ../Main.cpp
