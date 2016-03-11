# For non-shadow build use lib directory inside source tree:
isEmpty(ACFDIRBUILD){
	LIBS += -L$$OUT_PWD/../../../Lib/$$COMPILER_DIR
	LIBS += -L$$PWD/../../Lib/$$COMPILER_DIR
}

# Set lib directory for shadow build:
!isEmpty(ACFDIRBUILD){
	LIBS += -L$$ACFDIRBUILD/Lib/$$COMPILER_DIR
}

LIBS += -liqt -lipackage -lidoc -lifile -liimg -licmm -li2d -li3d -liprm -lilog -libase -licomp -liattr -liser -limod -limath -listd

# Used in istd::CSystem::ConvertLocalPathToUnc. Remove it after complete switching to Qt 5.5 and using of QStorageInfo!
win32{
	LIBS += -lMpr
}

