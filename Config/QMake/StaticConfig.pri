# Standard settings for an ACF static library

include(GeneralConfig.pri)

TEMPLATE = lib

CONFIG += staticlib
CONFIG += create_prl link_prl

INCLUDEPATH += $$_PRO_FILE_PWD_/../../
INCLUDEPATH += $$OUT_PWD/../../

DESTDIR = $$OUT_PWD/../../../Lib/$$COMPILER_DIR
HEADERS += $$files($$_PRO_FILE_PWD_/../*.h, false)
SOURCES += $$files($$_PRO_FILE_PWD_/../*.cpp, false)

isEqual(DEBUG_INFO_ENABLED_FOR_RELEASE, true){
	include(MsvcEnableDebugInfo.pri)
}
