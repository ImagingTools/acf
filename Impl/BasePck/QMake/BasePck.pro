include(../../../Config/QMake/Component.config)
include(../../AcfStd/QMake/AcfStd.pri)

TARGET = BasePck

win32-msvc*{
	LIBS += -lAcfStd
}
else{
	LIBS += $$ACFSTD_LIBS
}

QT -= gui core

HEADERS += ../BasePck.h
SOURCES += ../BasePck.cpp
