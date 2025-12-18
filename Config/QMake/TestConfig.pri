# Standard settings for an ACF test
include(GeneralConfig.pri)

TEMPLATE = app

QT = testlib
CONFIG += testcase
CONFIG += console

# For non-shadow build use lib directory inside source tree:
isEmpty(ACFDIRBUILD){
	LIBS += -L$$OUT_PWD/../../../Lib/$$COMPILER_DIR
	LIBS += -L$$PWD/../../Lib/$$COMPILER_DIR
}

# Set lib directory for shadow build:
!isEmpty(ACFDIRBUILD){
	LIBS += -L$$ACFDIRBUILD/Lib/$$COMPILER_DIR
}

LIBS += -liser -listd -litest

DESTDIR = $$OUT_PWD/../../../../Bin/$$COMPILER_DIR/Tests
HEADERS += $$files($$_PRO_FILE_PWD_/../*.h, false)
SOURCES += $$files($$_PRO_FILE_PWD_/../*.cpp, false)

isEqual(DEBUG_INFO_ENABLED_FOR_RELEASE, true){
	include($$(ACFCONFIGDIR)/QMake/MsvcEnableDebugInfo.pri)
}

win32:QMAKE_POST_LINK = "$$(QTDIR)\\bin\\windeployqt $$system_path($${DESTDIR})\\$${TARGET}.exe>NUL $$escape_expand(\n\t)"