TARGET = AcfLoc

include(../../../Config/QMake/StaticConfig.pri)

HEADERS =
SOURCES =
GENERATED_RESOURCES += $$_PRO_FILE_PWD_/../*.qrc

ACF_TRANSLATIONS += $$_PRO_FILE_PWD_/../*.ts

include(../../../Config/QMake/CustomBuild.pri)
