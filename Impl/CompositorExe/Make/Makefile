# Makefile to create the Acf runtime application
OUTPUT_NAME= AcfCompositor
ACFDIR=../../..
BINARY_DIR = $(ACFDIR)/Bin

include ../../Make/MakefileQt.settings
include ../../Make/Makefile.settings

LIB_DIR = ../../../Include
MAKE_FOLDER = Make
AFCSTD= AcfStd
AFCQT= AcfQt

#define the search libraries pathes
LIB_PATH = \
-L$(LIB_DIR)/ibase/$(MAKE_FOLDER)\
-L$(LIB_DIR)/iqt/$(MAKE_FOLDER)\
-L$(LIB_DIR)/icmpstr/$(MAKE_FOLDER)\
-L$(LIB_DIR)/iqtdoc/$(MAKE_FOLDER)\
-L$(LIB_DIR)/iqtgui/$(MAKE_FOLDER)\
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
-licmpstr \
-liqtdoc \
-liqtgui \
-liqt \
-lidoc \
-li2d \
-licomp \
-liser \
-listd \
-limod \
-libase \
-lisys \
-lQtGui \
-lQtCore

LINKER_FLAGS= -g 


all: $(OUTPUT_NAME)

# build objects
$(OUTPUT_NAME): $(LIB_OBJS) $(OBJ_DIR)

# clean objects
clean:
	@echo "  CLEAN  "$(OUTPUT_NAME)
	$(Q)rm -f $(LIB_OBJS)
	$(Q)rm -f $(AFCEXE)
   
# rules for creating the object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "     CC  "$<
	$(Q)mkdir -p $(OBJ_DIR)
	$(Q)$(CXX) ../Main.cpp -o $(OUTPUT_NAME) $(DEFAULT_COMPILER_FLAGS) $(ADDITIONAL_COMPILER_FLAGS) $(LIB_PATH) $(LIBS) $(LINKER_FLAGS)
	$(Q)mkdir -p $(BINARY_DIR)
	$(Q)cp $(OUTPUT_NAME) $(BINARY_DIR)/$(OUTPUT_NAME)
