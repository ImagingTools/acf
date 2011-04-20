include(../../../Config/QMake/ApplicationConfig.pri)
include(../../../Config/QMake/QtBaseConfig.pri)
include(../../AcfStd/QMake/AcfStd.pri)
include(../../AcfQt/QMake/AcfQt.pri)

TARGET = Arxc

mac*{
	CONFIG -= app_bundle
}

LIBS += -licmpstr

SOURCES += ../Main.cpp
