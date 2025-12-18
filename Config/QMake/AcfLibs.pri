# For non-shadow build use lib directory inside source tree:
isEmpty(ACFDIRBUILD){
	LIBS += -L$$OUT_PWD/../../../Lib/$$COMPILER_DIR
	LIBS += -L$$PWD/../../Lib/$$COMPILER_DIR
}

# Set lib directory for shadow build:
!isEmpty(ACFDIRBUILD){
	LIBS += -L$$ACFDIRBUILD/Lib/$$COMPILER_DIR
}

LIBS += -lipackage -lidoc -lifile -liimg -licmm -liqt -liprm -lilog -li2d -li3d -libase -licomp -limath -liser -liattr -limod -listd


# Used in istd::CSystem::ConvertLocalPathToUnc. Remove it after complete switching to Qt 5.5 and using of QStorageInfo!
win32{
	LIBS += -lMpr
}

