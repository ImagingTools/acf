# Makefile to create the wonderfull ACF framework
LIB_DIR = ..
MAKE_FOLDER = Make

all: AcfStd AcfQt Acf AcfCompositor

AcfStd: 
	$(MAKE) -C  $(LIB_DIR)/AcfStd/$(MAKE_FOLDER)
AcfQt: 
	$(MAKE) -C  $(LIB_DIR)/AcfQt/$(MAKE_FOLDER)
Acf: 
	$(MAKE) -C  $(LIB_DIR)/AcfExe/$(MAKE_FOLDER)
AcfCompositor: 
	$(MAKE) -C  $(LIB_DIR)/CompositorExe/$(MAKE_FOLDER)

clean:
	$(MAKE) -C  $(LIB_DIR)/AcfStd/$(MAKE_FOLDER) clean
	$(MAKE) -C  $(LIB_DIR)/AcfQt/$(MAKE_FOLDER) clean
	$(MAKE) -C  $(LIB_DIR)/AcfExe/$(MAKE_FOLDER) clean
	$(MAKE) -C  $(LIB_DIR)/CompositorExe/$(MAKE_FOLDER) clean
