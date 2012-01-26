include(../../../Config/QMake/StaticConfig.pri)
include(../../../Config/QMake/CustomBuild.pri)

TARGET = AcfLoc


HEADERS =
SOURCES =
COPYQMTOGENERATE += ../Acf_de_DE.ts
COPYQMTOGENERATE += ../Acf_en_US.ts
COPYQMTOGENERATE += ../Acf_pl_PL.ts
COPYQMTOGENERATE += ../Acf_ru_RU.ts
TRANSLATIONS += ../*.ts
RESOURCES += ../*.qrc
