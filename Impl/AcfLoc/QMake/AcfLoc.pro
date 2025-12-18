TARGET = AcfLoc

include(../../../Config/QMake/StaticConfig.pri)

HEADERS =
SOURCES =
RESOURCES += $$files($$_PRO_FILE_PWD_/../*.qrc, false)

TRANSLATIONS += $$files($$_PRO_FILE_PWD_/../Translations/*.ts, false)

# Set configuration of custom builds:
# ARX Compiler:
ARXC_CONFIG = $$PWD/../../../Config/AcfConfig.awc
ARXC_FILES += $$files($$_PRO_FILE_PWD_/../*.acc, false)
ARXC_OUTDIR = $$OUT_PWD/$$AUXINCLUDEPATH/GeneratedFiles/$$TARGET


# Get build output directory of shadow build:
ACFDIRBUILD = $$(ACFDIR_BUILD)
!isEmpty(ACFDIRBUILD){
	INCLUDEPATH += $$(ACFDIR_BUILD)/$$AUXINCLUDEDIR
}

include(../../../Config/QMake/CustomBuild.pri)

