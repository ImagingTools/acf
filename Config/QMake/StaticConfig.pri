# Standard settings for an ACF static library

include(GeneralConfig.pri)

TEMPLATE = lib

CONFIG += staticlib
CONFIG += create_prl link_prl

INCLUDEPATH += ../../
DESTDIR = ../../../Lib/$$COMPILER_DIR

HEADERS += ../*.h
SOURCES += ../*.cpp


win32-msvc*{
	# activate debug info also for release builds
	QMAKE_CXXFLAGS += /Zi /Fd$$DESTDIR/"$$TARGET".pdb
}

