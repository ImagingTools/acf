include(../../../Config/QMake/ApplicationConfig.pri)
include(../../../Config/QMake/QtBaseConfig.pri)
include(../../../Config/QMake/AcfStd.pri)
include(../../../Config/QMake/AcfQt.pri)

TARGET = Compositor

RESOURCES += ../*.qrc

LIBS += -licmpstr
LIBS += -lAcfLoc

HEADERS -= ../*.h

win32-msvc*{
	QMAKE_CXXFLAGS += /wd4264
}

QT += xml

ARXC_CONFIG = ../../../Config/Core.xpc
ARXC_FILES += ../*.arx

mac{
	ICON += ../Mac/Compositor.icns
	QMAKE_INFO_PLIST = ../Mac/Info.plist
}

#exists(../Generated/CCompositor.pri){
#	include(../Generated/CCompositor.pri)
#}

include(../../../Config/QMake/CustomBuild.pri)

