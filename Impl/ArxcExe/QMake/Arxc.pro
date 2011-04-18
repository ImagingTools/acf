include(../../../Config/QMake/Application.config)
include(../../../Config/QMake/QtBase.config)
include(../../AcfStd/QMake/AcfStd.pri)
include(../../AcfQt/QMake/AcfQt.pri)

TARGET = Arxc

LIBS += -licmpstr

SOURCES += ../Main.cpp
