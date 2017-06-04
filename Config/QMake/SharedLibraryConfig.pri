#Standard settings for an ACF shared library project

include (GeneralConfig.pri)

INCLUDEPATH += $$OUT_PWD/../../../Include
INCLUDEPATH += $$_PRO_FILE_PWD_/../../../Include
INCLUDEPATH += $$PWD/../../Impl

TEMPLATE = lib

DESTDIR = $$OUT_PWD/../../../Bin/$$COMPILER_DIR

CONFIG += link_prl
CONFIG += shared

!win32-msvc*{
	QMAKE_CFLAGS += -fPIC
}

HEADERS += $$_PRO_FILE_PWD_/../*.h
SOURCES += $$_PRO_FILE_PWD_/../*.cpp

isEqual(DEBUG_INFO_ENABLED_FOR_RELEASE, true){
	include(MsvcEnableDebugInfo.pri)
}
