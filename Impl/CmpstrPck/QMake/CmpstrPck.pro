include(../../../Config/QMake/Component.config)
include(../../../Config/QMake/QtBase.config)
include(../../AcfStd/QMake/AcfStd.pri)
include(../../AcfQt/QMake/AcfQt.pri)

TARGET = CmpstrPck

win32-msvc*{
	LIBS += -licmpstr -lAcfQt -lAcfStd
}
else{
	LIBS += -licmpstr $$ACFQT_LIBS $$ACFSTD_LIBS
}

QT += xml

HEADERS += ../CmpstrPck.h
SOURCES += ../CmpstrPck.cpp
