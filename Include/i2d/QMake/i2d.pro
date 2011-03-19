TARGET = i2d
TEMPLATE = lib
CONFIG += staticlib

CONFIG(debug, debug|release) {
        DESTDIR = ../../../Lib/DebugQMake
}
CONFIG(release, debug|release) {
        DESTDIR = ../../../Lib/ReleaseQMake
}

INCLUDEPATH += ../../ 

HEADERS += ../*.h
SOURCES += ../*.cpp
