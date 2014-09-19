TARGET = ibase

include(../../../Config/QMake/StaticConfig.pri)

mac{
	LIBS += -framework Carbon
}
