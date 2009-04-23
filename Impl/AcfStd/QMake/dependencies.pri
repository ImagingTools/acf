CONFIG += lib_bundle
CONFIG += no_lflags_merge

INCLUDEPATH = ../../../Include

CONFIG(debug, debug|release) {
	LIBS += -L../../../Lib/DebugQMake
}
CONFIG(release, debug|release) {
	LIBS += -L../../../Lib/ReleaseQMake
}
SOURCES += ../../../Include/istd/QMake/debug/CBitManip.o
