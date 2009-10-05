TARGET = iqaxmm
TEMPLATE = lib
CONFIG += staticlib

CONFIG(debug, debug|release) {
	DESTDIR = ..\..\..\Lib\DebugQMake
}
CONFIG(release, debug|release) {
	DESTDIR = ..\..\..\Lib\ReleaseQMake
}

UI_DIR = ../Generated
MOC_DIR = ../Generated
RCC_DIR = ../Generated

QT += 


INCLUDEPATH += ../../ "$(QTDIR)/include" "$(QTDIR)/include/QtCore" "$(QTDIR)/include/QtGui" "$(QTDIR)/include/QtXml" "$(QTDIR)/include/phonon" "$(QTDIR)/include/ActiveQt" 

HEADERS += ../CVlcVideoViewGuiComp.h
HEADERS += ../iqaxmm.h
HEADERS += ../Wrapped\axvlc.h
SOURCES += ../CVlcVideoViewGuiComp.cpp
SOURCES += ../Wrapped\axvlc.cpp
