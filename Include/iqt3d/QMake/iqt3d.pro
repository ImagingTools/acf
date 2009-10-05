TARGET = iqt3d
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


INCLUDEPATH += ../../ "$(QTDIR)/include" "$(QTDIR)/include/QtCore" "$(QTDIR)/include/QtGui" "$(QTDIR)/include/QtXml" "$(QWT3DDIR)/Include" 

HEADERS += ../CSurfaceViewComp.h
SOURCES += ../CSurfaceViewComp.cpp
SOURCES += ../generated\moc_CSurfaceViewComp.cpp
FORMS += ../CSurfaceViewComp.ui
