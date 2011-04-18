include($(ACFDIR)/Config/QMake/Static.config)
include($(ACFDIR)/Config/QMake/QtGuiBase.config)

TARGET = DocView

DESTDIR = ../../../../../Lib/$$COMPILER_DIR

INCLUDEPATH += ..

HEADERS += ../*.h
SOURCES += ../*.cpp
