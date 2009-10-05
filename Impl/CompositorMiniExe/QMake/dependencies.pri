CONFIG += lib_bundle

INCLUDEPATH = ../../../Include

CONFIG(debug, debug|release) {
	LIBS += -L../../../Lib/DebugQMake
}
CONFIG(release, debug|release) {
	LIBS += -L../../../Lib/ReleaseQMake
}
LIBS += -lisys -listd -liser -liqtprm -liqtgui -liqtdoc -liqt2d -liqt -liprm -limod -limath -liimg -lidoc -lidb -licomp -licmpstr -libase -li2d
