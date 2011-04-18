include(../../../Config/QMake/Component.config)
include(../../../Config/QMake/QtBase.config)
include(../../AcfStd/QMake/AcfStd.pri)
include(../../AcfQt/QMake/AcfQt.pri)

TARGET = CmpstrPck

LIBS += -licmpstr

QT += xml

HEADERS += ../CmpstrPck.h
SOURCES += ../CmpstrPck.cpp
