include(../../../Config/QMake/StaticConfig.pri)
include(../../../Config/QMake/QtGuiBaseConfig.pri)

TARGET = iqt2d

LIBS += -liqt
LIBS += -li2d

RESOURCES += ../*.qrc
