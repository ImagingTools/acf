TARGET = QtMmPck
TEMPLATE = lib

CONFIG += dll

TARGET_EXT = .arp

CONFIG(debug, debug|release) {
        DESTDIR = ../../../Bin/DebugQMake
        LIBS += -L"$(ACFDIR)/Lib/DebugQMake
        LIBS += AcfStd.lib AcfQt.lib phonond4.lib
}
CONFIG(release, debug|release) {
        DESTDIR = ../../../Bin/ReleaseQMake
        LIBS += -L"$(ACFDIR)/Lib/ReleaseQMake
        LIBS += AcfStd.lib AcfQt.lib phonon4.lib
}

UI_DIR = ../Generated
MOC_DIR = ../Generated
RCC_DIR = ../Generated

QT += main core gui svg 

INCLUDEPATH += ../../../Include "$(QTDIR)/include" "$(QTDIR)/include/QtCore" "$(QTDIR)/include/QtGui" "$(QTDIR)/include/QtXml" "$(QTDIR)/include/phonon" "$(QTDIR)/include/QtSvg" 

HEADERS += ../QtMmPck.h
SOURCES += ../QtMmPck.cpp
