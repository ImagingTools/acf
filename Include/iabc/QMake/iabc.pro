TARGET = iabc
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


INCLUDEPATH += ../../ "$(QTDIR)/include" "$(QTDIR)/include/QtCore" "$(QTDIR)/include/QtGui" "$(QTDIR)/include/QtXml" "$(QTSERVICEDIR)/Src" 

HEADERS += ../CFolderMonitorComp.h
HEADERS += ../CServiceApplicationComp.h
HEADERS += ../iabc.h
SOURCES += ../CFolderMonitorComp.cpp
SOURCES += ../CServiceApplicationComp.cpp
SOURCES += ../generated\moc_CFolderMonitorComp.cpp
SOURCES += ../generated\moc_CServiceApplicationComp.cpp
