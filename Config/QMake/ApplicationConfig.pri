#Standard settings for an ACF application

include (GeneralConfig.pri)

TEMPLATE = app

!macx-ios*{
	CONFIG += link_prl
	CONFIG += app_bundle
}

INCLUDEPATH += $$PWD/../../Impl
INCLUDEPATH += $$OUT_PWD/../../

DESTDIR = $$OUT_PWD/../../../Bin/$$COMPILER_DIR

HEADERS += $$_PRO_FILE_PWD_/../*.h
SOURCES += $$_PRO_FILE_PWD_/../*.cpp

isEqual(DEBUG_INFO_ENABLED_FOR_RELEASE, true){
	include(MsvcEnableDebugInfo.pri)
}


