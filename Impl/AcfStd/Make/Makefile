# Makefile to create AcfStd library
SOURCE_DIR = ../../../Include
MAKE_FOLDER = Make
ACFSTD_OBJECTS = \
$(SOURCE_DIR)/i2d/$(MAKE_FOLDER)/libi2d.a \
$(SOURCE_DIR)/ibase/$(MAKE_FOLDER)/libibase.a \
$(SOURCE_DIR)/icmm/$(MAKE_FOLDER)/libicmm.a \
$(SOURCE_DIR)/icomp/$(MAKE_FOLDER)/libicomp.a \
$(SOURCE_DIR)/idoc/$(MAKE_FOLDER)/libidoc.a \
$(SOURCE_DIR)/iimg/$(MAKE_FOLDER)/libiimg.a \
$(SOURCE_DIR)/imath/$(MAKE_FOLDER)/libimath.a \
$(SOURCE_DIR)/imeas/$(MAKE_FOLDER)/libimeas.a \
$(SOURCE_DIR)/imod/$(MAKE_FOLDER)/libimod.a \
$(SOURCE_DIR)/iprm/$(MAKE_FOLDER)/libiprm.a \
$(SOURCE_DIR)/iser/$(MAKE_FOLDER)/libiser.a \
$(SOURCE_DIR)/istd/$(MAKE_FOLDER)/libistd.a \
$(SOURCE_DIR)/isys/$(MAKE_FOLDER)/libisys.a

all: AcfStd

AcfStd: i2d ibase icmm icomp idb idoc iimg imath imeas imod inat iprm iser istd isvn isys AcfStd.a

AcfStd.a: $(ACFSTD_OBJECTS)

i2d:
	$(MAKE) -C  $(SOURCE_DIR)/i2d/$(MAKE_FOLDER)
ibase:
	$(MAKE) -C  $(SOURCE_DIR)/ibase/$(MAKE_FOLDER)
icmm:
	$(MAKE) -C  $(SOURCE_DIR)/icmm/$(MAKE_FOLDER)
icomp:
	$(MAKE) -C  $(SOURCE_DIR)/icomp/$(MAKE_FOLDER)
idb:
	$(MAKE) -C  $(SOURCE_DIR)/idb/$(MAKE_FOLDER)
idoc:
	$(MAKE) -C  $(SOURCE_DIR)/idoc/$(MAKE_FOLDER)
iimg:
	$(MAKE) -C  $(SOURCE_DIR)/iimg/$(MAKE_FOLDER)
imath:
	$(MAKE) -C  $(SOURCE_DIR)/imath/$(MAKE_FOLDER)
imeas:
	$(MAKE) -C  $(SOURCE_DIR)/imeas/$(MAKE_FOLDER)
imod:
	$(MAKE) -C  $(SOURCE_DIR)/imod/$(MAKE_FOLDER)
inat:
	$(MAKE) -C  $(SOURCE_DIR)/inat/$(MAKE_FOLDER)
iprm:
	$(MAKE) -C  $(SOURCE_DIR)/iprm/$(MAKE_FOLDER)
iser:
	$(MAKE) -C  $(SOURCE_DIR)/iser/$(MAKE_FOLDER)
istd:
	$(MAKE) -C  $(SOURCE_DIR)/istd/$(MAKE_FOLDER)
isvn:
	$(MAKE) -C  $(SOURCE_DIR)/isvn/$(MAKE_FOLDER)
isys:
	$(MAKE) -C  $(SOURCE_DIR)/isys/$(MAKE_FOLDER)

clean:
	$(MAKE) -C $(SOURCE_DIR)/i2d/$(MAKE_FOLDER) clean
	$(MAKE) -C $(SOURCE_DIR)/ibase/$(MAKE_FOLDER) clean
	$(MAKE) -C $(SOURCE_DIR)/idb/$(MAKE_FOLDER) clean
	$(MAKE) -C $(SOURCE_DIR)/idoc/$(MAKE_FOLDER) clean
	$(MAKE) -C $(SOURCE_DIR)/iimg/$(MAKE_FOLDER) clean
	$(MAKE) -C $(SOURCE_DIR)/imath/$(MAKE_FOLDER) clean
	$(MAKE) -C $(SOURCE_DIR)/imeas/$(MAKE_FOLDER) clean
	$(MAKE) -C $(SOURCE_DIR)/imod/$(MAKE_FOLDER) clean
	$(MAKE) -C $(SOURCE_DIR)/inat/$(MAKE_FOLDER) clean
	$(MAKE) -C $(SOURCE_DIR)/iprm/$(MAKE_FOLDER) clean
	$(MAKE) -C $(SOURCE_DIR)/iser/$(MAKE_FOLDER) clean
	$(MAKE) -C $(SOURCE_DIR)/istd/$(MAKE_FOLDER) clean
	$(MAKE) -C $(SOURCE_DIR)/isvn/$(MAKE_FOLDER) clean
	$(MAKE) -C $(SOURCE_DIR)/isys/$(MAKE_FOLDER) clean

# rules for creating the Acf standard library:
AcfStd.a: $(ACFSTD_OBJECTS)
	@echo "     AR  "$@
	$(Q)ar -q $@ $(ACFSTD_OBJECTS)
