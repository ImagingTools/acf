TARGET = Carxc

include($(ACFCONFIGDIR)/QMake/ApplicationConfig.pri)
include($(ACFCONFIGDIR)/QMake/QtBaseConfig.pri)

INCLUDEPATH += ../../../Include
INCLUDEPATH += ../../../Impl

LIBS += -lAcfLoc

HEADERS -= ../*.h

win32-msvc*{
	QMAKE_CXXFLAGS += /wd4264
}

QT += xml
greaterThan(QT_MAJOR_VERSION, 4): QT += printsupport


# configuration of custom builds

# ARX compiler
ARXC_CONFIG = ../../../Config/Core.xpc
ARXC_FILES += ../*.arx
ARXC_OUTDIR = $$AUXINCLUDEPATH/GeneratedFiles/$$TARGET

win*{
	# File transformation
	ACF_CONVERT_FILES = ../VC/*.rc.xtracf
	ACF_CONVERT_OUTDIR = $$AUXINCLUDEPATH/GeneratedFiles/$$TARGET
	ACF_CONVERT_REGISTRY = ../../../Partitura/AcfInfoCopyApp.arx
	ACF_CONVERT_CONFIG = ../../../Config/BaseOnly.xpc

	RC_FILE = $$AUXINCLUDEPATH/GeneratedFiles/$$TARGET/Carxc.rc
}


include($(ACFCONFIGDIR)/QMake/CustomBuild.pri)
include($(ACFCONFIGDIR)/QMake/AcfQt.pri)
include($(ACFCONFIGDIR)/QMake/AcfStd.pri)
