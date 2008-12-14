TARGET = AcfStd
TEMPLATE = subdirs
CONFIG += staticlib
CONFIG += create_prl
CONFIG += link_prl
SUBDIRS = ../../../Include/istd/QtCreator += ../../../Include/iser/QtCreator
PRE_TARGETDEPS += istd iser
SOURCES += ../Empty.cpp
