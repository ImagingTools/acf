TARGET = icompTest

include(../../../../Config/QMake/TestConfig.pri)
include(../../../../Config/QMake/QtBaseConfig.pri)

LIBS += -licomp -lifile -lilog -liser -limod -listd -lipackage


# Set configuration of custom builds:
# ARX Compiler:
ARXC_CONFIG = $$PWD/../../../Config/AcfTestConfig.awc
ARXC_FILES += $$files($$_PRO_FILE_PWD_/../*.acc, false)
ARXC_OUTDIR = $$OUT_PWD/$$AUXINCLUDEPATH/GeneratedFiles/$$TARGET

include($(ACFCONFIGDIR)/QMake/AcfQt.pri)
include($(ACFCONFIGDIR)/QMake/AcfStd.pri)
include($(ACFCONFIGDIR)/QMake/CustomBuild.pri)
