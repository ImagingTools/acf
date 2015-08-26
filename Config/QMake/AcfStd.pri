# get build output directory of shadow build
ACFDIRBUILD = $$(ACFDIR_BUILD)

# for non-shadow build use lib directory inside source tree
isEmpty( ACFDIRBUILD ) {
	LIBS += -L$$OUT_PWD/../../../Lib/$$COMPILER_DIR
	LIBS += -L$$PWD/../../Lib/$$COMPILER_DIR
}

# set lib directory for shadow build
!isEmpty( ACFDIRBUILD ) {
	LIBS += -L$$(ACFDIR_BUILD)/Lib/$$COMPILER_DIR
}

LIBS += -liqt -lipackage -lidoc -lifile -liimg -licmm -li2d -libase -liprm -lilog -licomp -liser -limod -limath -listd

