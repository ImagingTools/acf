TARGET = Arxc

include(../../../Config/QMake/ApplicationConfig.pri)
include(../../../Config/QMake/QtBaseConfig.pri)

win32-msvc*{
	# copying all Qt DLLs to destination directory
	greaterThan(QT_MAJOR_VERSION, 4): QMAKE_POST_LINK = $(QTDIR)\bin\windeployqt $$DESTDIR
}
mac*{
	CONFIG -= app_bundle
}

CONFIG += console

HEADERS =

LIBS += -lipackage

include(../../../Config/QMake/AcfStd.pri)
include(../../../Config/QMake/AcfQt.pri)
