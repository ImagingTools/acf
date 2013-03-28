include(../../../Config/QMake/ApplicationConfig.pri)
include(../../../Config/QMake/QtBaseConfig.pri)

TARGET = Compositor

RESOURCES += ../*.qrc

LIBS += -licmpstr
LIBS += -lAcfLoc

HEADERS -= ../*.h

win32-msvc*{
	QMAKE_CXXFLAGS += /wd4264
}

QT += xml

mac{
	ICON += ../Mac/Compositor.icns
	QMAKE_INFO_PLIST = ../Mac/Info.plist
}


# configuration of custom builds

# ARX compiler
ARXC_CONFIG = ../../../Config/Core.xpc
ARXC_FILES += ../*.arx

win*{
	# File transformation
	ACF_CONVERT_FILES = ../VC/*.rc.xtracf
	ACF_CONVERT_OUTDIR = ../Generated
	ACF_CONVERT_REGISTRY = ../../../Partitura/AcfInfoCopyApp.arx
	ACF_CONVERT_CONFIG = ../../../Config/BaseOnly.xpc

	RC_FILE = ../Generated/Compositor.rc
}

#exists(../Generated/CCompositor.pri){
#	include(../Generated/CCompositor.pri)
#}

include(../../../Config/QMake/CustomBuild.pri)
include(../../../Config/QMake/AcfQt.pri)
include(../../../Config/QMake/AcfStd.pri)
