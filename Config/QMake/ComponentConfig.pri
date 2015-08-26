#Standard settings for an ACF package project

include (GeneralConfig.pri)

INCLUDEPATH += $$OUT_PWD/../../../Include
INCLUDEPATH += $$_PRO_FILE_PWD_/../../../Include

TEMPLATE = lib

TARGET_EXT = .arp

DESTDIR = $$OUT_PWD/../../../Bin/$$COMPILER_DIR

CONFIG += link_prl

!win32-msvc*{
	CONFIG += plugin no_plugin_name_prefix
	QMAKE_CFLAGS += -fPIC
	QMAKE_EXTENSION_SHLIB = arp
}
else{
	CONFIG += dll
}

HEADERS += $$_PRO_FILE_PWD_/../*.h
SOURCES += $$_PRO_FILE_PWD_/../*.cpp

isEqual(DEBUG_INFO_ENABLED_FOR_RELEASE, true){
	include(MsvcEnableDebugInfo.pri)
}
