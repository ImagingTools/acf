#Standard settings for an ACF application

include (GeneralConfig.pri)

TEMPLATE = app

CONFIG += link_prl
CONFIG += app_bundle

INCLUDEPATH += $$PWD/../../Impl

DESTDIR = ../../../Bin/$$COMPILER_DIR

HEADERS += ../*.h
SOURCES += ../*.cpp

win32-msvc*{
	# activate debug info also for release builds
	QMAKE_CXXFLAGS += /Zi /Fd$$DESTDIR/"$$TARGET".pdb
}
