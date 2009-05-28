TARGET = Compositor
TEMPLATE = app

CONFIG(debug, debug|release) {
	DESTDIR = ../../../Bin/DebugQMake
        LIBS += -L"$(ACFDIR)/Lib/DebugQMake"
}
CONFIG(release, debug|release) {
	DESTDIR = ../../../Bin/ReleaseQMake
        LIBS += -L"$(ACFDIR)/Lib/ReleaseQMake"
}

UI_DIR = ../Generated
MOC_DIR = ../Generated
RCC_DIR = ../Generated

QT += core gui xml sql network

include(dependencies.pri)

INCLUDEPATH += "$(ACFDIR)/Include" "$(ACFDIR)/Impl" "$(QTDIR)/include" "$(QTDIR)/include/QtCore" "$(QTDIR)/include/QtGui" "$(QTDIR)/include/QtXml" "$(QTDIR)/include/QtScript" .. 

SOURCES += ../Main.cpp
