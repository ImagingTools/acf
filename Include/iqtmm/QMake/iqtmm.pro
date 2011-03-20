TARGET = iqtmm
TEMPLATE = lib
CONFIG += staticlib
CONFIG += stl

CONFIG(debug, debug|release) {
	DESTDIR = ../../../Lib/DebugQMake
}
CONFIG(release, debug|release) {
	DESTDIR = ../../../Lib/ReleaseQMake
}

UI_DIR = ../Generated
MOC_DIR = ../Generated
RCC_DIR = ../Generated

INCLUDEPATH += ../../ "$(QTDIR)/include" "$(QTDIR)/include/QtCore" "$(QTDIR)/include/QtGui" "$(QTDIR)/include/QtXml" "$(QTDIR)/include/QtSvg" "$(QTDIR)/include/phonon" 

HEADERS += ../*.h
SOURCES += ../*.cpp
FORMS += ../*.ui
