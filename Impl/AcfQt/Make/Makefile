# Makefile to create the AcfQt library
SOURCE_DIR = ../../../Include
MAKE_FOLDER = Make
ACFQT_OBJECTS = \
$(SOURCE_DIR)/iqt/$(MAKE_FOLDER)/libiqt.a \
$(SOURCE_DIR)/iqt2d/$(MAKE_FOLDER)/libiqt2d.a \
$(SOURCE_DIR)/iqtdoc/$(MAKE_FOLDER)/libiqtdoc.a \
$(SOURCE_DIR)/iqtgui/$(MAKE_FOLDER)/libiqtgui.a \
$(SOURCE_DIR)/iqtprm/$(MAKE_FOLDER)/libiqtprm.a

all: AcfQt

AcfQt: iqt iqt2d iqtdoc iqtgui iqtprm AcfQt.a

AcfQt.a: $(ACFQT_OBJECTS)

iqt:
	$(MAKE) -C  $(SOURCE_DIR)/iqt/$(MAKE_FOLDER)
iqt2d:
	$(MAKE) -C  $(SOURCE_DIR)/iqt2d/$(MAKE_FOLDER)
iqtdoc:
	$(MAKE) -C  $(SOURCE_DIR)/iqtdoc/$(MAKE_FOLDER)
iqtgui:
	$(MAKE) -C  $(SOURCE_DIR)/iqtgui/$(MAKE_FOLDER)
iqtprm:
	$(MAKE) -C  $(SOURCE_DIR)/iqtprm/$(MAKE_FOLDER)

clean:
	$(MAKE) -C $(SOURCE_DIR)/iqt/$(MAKE_FOLDER) clean
	$(MAKE) -C $(SOURCE_DIR)/iqt2d/$(MAKE_FOLDER) clean
	$(MAKE) -C $(SOURCE_DIR)/iqtdoc/$(MAKE_FOLDER) clean
	$(MAKE) -C $(SOURCE_DIR)/iqtgui/$(MAKE_FOLDER) clean
	$(MAKE) -C $(SOURCE_DIR)/iqtprm/$(MAKE_FOLDER) clean
	
# rules for creating the Acf standard library:
AcfQt.a: $(ACFQT_OBJECTS)
	@echo "     AR  "$@
	$(Q)ar -q $@ $(ACFQT_OBJECTS)
