TARGET = iwidgets

include(../../../Config/QMake/StaticConfig.pri)
include(../../../Config/QMake/QtGuiBaseConfig.pri)

FORMS += $$files($$_PRO_FILE_PWD_/../*.ui, false)

