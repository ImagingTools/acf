include(../../../Config/QMake/Component.config)
include(../../../Config/QMake/QtBase.config)
include(../../AcfStd/QMake/AcfStd.pri)
include(../../AcfQt/QMake/AcfQt.pri)

TARGET = QtMmPck

QT += svg phonon

HEADERS += ../QtMmPck.h
SOURCES += ../QtMmPck.cpp
