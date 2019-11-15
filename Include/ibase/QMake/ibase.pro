TARGET = ibase

include(../../../Config/QMake/StaticConfig.pri)

!macx-ios*{
	mac{
		LIBS += -framework Carbon
	}
}

