TARGET = Arxc

include(../../../Config/QMake/ApplicationConfig.pri)
include(../../../Config/QMake/QtBaseConfig.pri)

mac*{
	CONFIG -= app_bundle
}

CONFIG += console

HEADERS =

LIBS += -lipackage

include(../../../Config/QMake/AcfStd.pri)
include(../../../Config/QMake/AcfQt.pri)
