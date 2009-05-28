CONFIG += lib_bundle

INCLUDEPATH = ../../../Include

CONFIG(debug, debug|release) {
	LIBS += -L../../../Lib/DebugQMake
}
CONFIG(release, debug|release) {
	LIBS += -L../../../Lib/ReleaseQMake
}
LIBS += -lizlib -lixln -liwin -lisys -lisvn -listd -liser -liqtprm -liqtgui -liqtdoc -liqt2d -liqt -liprm -linat -limod -limeas -limath -liimg -lidoc -lidb -licomp -licmpstr -licmm -libase -li2d
