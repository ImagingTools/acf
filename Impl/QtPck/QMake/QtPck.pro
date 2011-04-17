include(../../../Config/QMake/Component.config)
include(../../../Config/QMake/QtBase.config)
include(../../AcfStd/QMake/AcfStd.pri)
include(../../AcfQt/QMake/AcfQt.pri)

TARGET = QtPck

win32-msvc*{
	LIBS += -lAcfQt -lAcfStd
}
else{
	LIBS += $$ACFQT_LIBS $$ACFSTD_LIBS
}

QT += xml

HEADERS += ../QtPck.h
SOURCES += ../QtPck.cpp
