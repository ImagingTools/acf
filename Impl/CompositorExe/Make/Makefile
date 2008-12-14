# Makefile to create the Acf Compositor application

BUILD_EXE = 1
OUTPUT_NAME= AcfCompositor
ACFDIR=../../..
BINARY_DIR = $(ACFDIR)/Bin

include ../../Make/Makefile.settings
include ../../Make/MakefileQt.settings

LIB_DIR = ../../../Include
MAKE_FOLDER = Make

#define the search libraries pathes
LIB_PATH = \
-L$(LIB_DIR)/ibase/$(MAKE_FOLDER)\
-L$(LIB_DIR)/iqt/$(MAKE_FOLDER)\
-L$(LIB_DIR)/iqtgui/$(MAKE_FOLDER)\
-L$(LIB_DIR)/iqtdoc/$(MAKE_FOLDER)\
-L$(LIB_DIR)/idoc/$(MAKE_FOLDER)\
-L$(LIB_DIR)/icomp/$(MAKE_FOLDER)\
-L$(LIB_DIR)/iser/$(MAKE_FOLDER)\
-L$(LIB_DIR)/istd/$(MAKE_FOLDER)\
-L$(LIB_DIR)/imod/$(MAKE_FOLDER)\
-L$(LIB_DIR)/i2d/$(MAKE_FOLDER)\
-L$(LIB_DIR)/isys/$(MAKE_FOLDER)\
-L$(QTDIR)/lib/


#define the libraries, which must be linked to the Acf executable
LIBS = \
-liqtdoc \
-liqtgui \
-liqt \
-lidoc \
-licomp \
-li2d \
-libase \
-liser \
-limod \
-listd \
-lisys \
-lQtGui \
-lQtCore

HAS_UI = 1
HAS_MOC = 1
BUILD_EXECUTABLE = 1

# include the basic rules for creation this module
include ../../../Impl/Make/Makefile.rules

# include the rules for compilation of Qt related classes
include ../../../Impl/Make/MakefileQt.rules

# include the rules for compilation of Qt related classes
include ../../../Impl/Make/MakefileExecutable.rules
