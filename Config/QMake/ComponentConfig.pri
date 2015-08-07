#Standard settings for an ACF package project

include (GeneralConfig.pri)

INCLUDEPATH += ../../../Include

TEMPLATE = lib

TARGET_EXT = .arp

DESTDIR = ../../../Bin/$$COMPILER_DIR

CONFIG += link_prl

!win32-msvc*{
	CONFIG += plugin no_plugin_name_prefix
	QMAKE_CFLAGS += -fPIC
	QMAKE_EXTENSION_SHLIB = arp
}
else{
	CONFIG += dll
}

win32-msvc*{
	# activate debug info also for release builds
	QMAKE_CXXFLAGS += /Zi /Fd$$DESTDIR/"$$TARGET".pdb
}


HEADERS += ../*.h
SOURCES += ../*.cpp
