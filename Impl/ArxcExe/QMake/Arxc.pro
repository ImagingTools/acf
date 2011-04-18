include(../../../Config/QMake/General.config)
include(../../../Config/QMake/QtBase.config)
include(../../AcfStd/QMake/AcfStd.pri)
include(../../AcfQt/QMake/AcfQt.pri)

TARGET = Arxc
TEMPLATE = app
CONFIG += link_prl

CONFIG(debug, debug|release){
	DESTDIR = ../../../Bin/DebugQMake
	LIBS += -L../../../Lib/DebugQMake 
}
CONFIG(release, debug|release){
	DESTDIR = ../../../Bin/ReleaseQMake
	LIBS += -L../../../Lib/ReleaseQMake 
}

LIBS += -licmpstr

INCLUDEPATH += ../../../Include ../../../Impl

SOURCES += ../Main.cpp
