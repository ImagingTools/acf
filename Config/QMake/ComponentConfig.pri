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

	# due a bug in QMake we need manual renaming of the package files:
	linux{
		QMAKE_POST_LINK += mv $$DESTDIR/$${TARGET}.so $$DESTDIR/$${TARGET}.arp
	}
}
else{
	CONFIG += dll
}

HEADERS += $$files($$_PRO_FILE_PWD_/../*.h, false)
SOURCES += $$files($$_PRO_FILE_PWD_/../*.cpp, false)

isEqual(DEBUG_INFO_ENABLED_FOR_RELEASE, true){
	include(MsvcEnableDebugInfo.pri)
}

