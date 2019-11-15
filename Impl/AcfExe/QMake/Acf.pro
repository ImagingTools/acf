TARGET = Acf

include(../../../Config/QMake/ApplicationConfig.pri)
include(../../../Config/QMake/QtBaseConfig.pri)

mac*{
	CONFIG -= app_bundle
}

CONFIG += console

HEADERS =

LIBS += -lipackage

QT += xml

include(../../../Config/QMake/AcfQt.pri)
include(../../../Config/QMake/AcfStd.pri)

