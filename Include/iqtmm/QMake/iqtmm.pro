TARGET = iqtmm
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


INCLUDEPATH += ../../ "$(QTDIR)/include" "$(QTDIR)/include/QtCore" "$(QTDIR)/include/QtGui" "$(QTDIR)/include/QtXml" "$(QTDIR)/include/QtSvg" "$(QTDIR)/include/phonon" 

HEADERS += ../CFrameSeqVideoControllerComp.h
HEADERS += ../CMediaLoaderComp.h
HEADERS += ../CPhononVideoViewGuiComp.h
HEADERS += ../CPlaybackControllerGuiComp.h
HEADERS += ../CSvgBitmapLoaderComp.h
HEADERS += ../CVideoPlayerGuiComp.h
HEADERS += ../iqtmm.h
HEADERS += ../generated\ui_CPlaybackControllerGuiComp.h
HEADERS += ../generated\ui_CVideoPlayerGuiComp.h
SOURCES += ../CFrameSeqVideoControllerComp.cpp
SOURCES += ../CMediaLoaderComp.cpp
SOURCES += ../CPhononVideoViewGuiComp.cpp
SOURCES += ../CPlaybackControllerGuiComp.cpp
SOURCES += ../CSvgBitmapLoaderComp.cpp
SOURCES += ../CVideoPlayerGuiComp.cpp
SOURCES += ../generated\moc_CFrameSeqVideoControllerComp.cpp
SOURCES += ../generated\moc_CPhononVideoViewGuiComp.cpp
SOURCES += ../generated\moc_CPlaybackControllerGuiComp.cpp
FORMS += ../CPlaybackControllerGuiComp.ui
FORMS += ../CVideoPlayerGuiComp.ui
