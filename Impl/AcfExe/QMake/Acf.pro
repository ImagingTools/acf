include(../../../Config/QMake/Application.config)
include(../../../Config/QMake/QtBase.config)
include(../../AcfStd/QMake/AcfStd.pri)
include(../../AcfQt/QMake/AcfQt.pri)

TARGET = Acf

LIBS += -licmpstr

QT += xml

SOURCES += ../Main.cpp
