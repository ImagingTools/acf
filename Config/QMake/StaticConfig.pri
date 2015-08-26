# Standard settings for an ACF static library

include(GeneralConfig.pri)

TEMPLATE = lib

CONFIG += staticlib
CONFIG += create_prl link_prl

INCLUDEPATH += $$_PRO_FILE_PWD_/../../
INCLUDEPATH += $$OUT_PWD/../../

DESTDIR = $$OUT_PWD/../../../Lib/$$COMPILER_DIR
HEADERS += $$_PRO_FILE_PWD_/../*.h
SOURCES += $$_PRO_FILE_PWD_/../*.cpp

isEqual(DEBUG_INFO_ENABLED_FOR_RELEASE, true){
	include(MsvcEnableDebugInfo.pri)
}
