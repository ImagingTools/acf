TARGET = AcfLoc

include(../../../Config/QMake/StaticConfig.pri)

HEADERS =
SOURCES =
RESOURCES += $$_PRO_FILE_PWD_/../*.qrc

TRANSLATIONS += $$_PRO_FILE_PWD_/../Translations/*.ts

# Set configuration of custom builds:
# ARX Compiler:
ARXC_CONFIG = $$PWD/../../../Config/AcfConfig.awc
ARXC_FILES += $$PWD/../*.acc
ARXC_OUTDIR = $$OUT_PWD/$$AUXINCLUDEPATH/GeneratedFiles/$$TARGET


# Get build output directory of shadow build:
ACFDIRBUILD = $$(ACFDIR_BUILD)
!isEmpty(ACFDIRBUILD){
	INCLUDEPATH += $$(ACFDIR_BUILD)/$$AUXINCLUDEDIR
}


include(../../../Config/QMake/CustomBuild.pri)
