include (AcfLibs.pri)

LIBS += -lipackage -lidoc -lifile -liimg -licmm -liqt -liprm -lilog -li2d -li3d -libase -licomp -limath -liser -liattr -limod -listd


# Used in istd::CSystem::ConvertLocalPathToUnc. Remove it after complete switching to Qt 5.5 and using of QStorageInfo!
win32{
	LIBS += -lMpr
}

