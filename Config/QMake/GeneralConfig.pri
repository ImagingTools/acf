#General base configuration should be included from all ACF-based projects

CONFIG += stl
CONFIG -= exceptions
CONFIG += silent
CONFIG += rtti
CONFIG += debug_and_release

PLATFORM_CODE = i86
win32:contains(QMAKE_HOST.arch, x86_64) | *-64{
	PLATFORM_CODE = x64
}

CONFIG(release, debug|release){
	#sse support
	CONFIG += sse2
}

COMPILER_NAME = QMake
PLATFORM_NAME = Unix

 CONFIG += depend_includepath

win32-msvc*{
	COMPILER_NAME = VC
	PLATFORM_NAME = Win
	QMAKE_CXXFLAGS += /wd4127 /wd4250 /wd4347 /wd4355 /wd4365 /wd4481 /wd4505 /wd4510 /wd4511 /wd4512 /wd4548 /wd4571 /wd4619 /wd4625 /wd4626 /wd4640 /wd4702 /wd4710 /wd4820 /wd4826
	QMAKE_CXXFLAGS_WARN_ON = -W4
	QMAKE_CXXFLAGS += /MP /openmp- /fp:fast

		# Qt-BUG 31516. Remove it after the fix in Qt!!!
		QMAKE_CXXFLAGS += /wd4718

		win32-msvc2005{
		COMPILER_NAME = VC8
	}
	win32-msvc2008{
		COMPILER_NAME = VC9
		QMAKE_CXXFLAGS -= /Gd
		QMAKE_CXXFLAGS -= /GD
		QMAKE_CXXFLAGS -= -Gd
		QMAKE_CXXFLAGS -= -GD
	}
	win32-msvc2010{
		QMAKE_CXXFLAGS += /wd4996
		COMPILER_NAME = VC10
	}
	win32-msvc2012{
		QMAKE_CXXFLAGS += /wd4996
		COMPILER_NAME = VC11
	}
	win32-msvc2013{
		QMAKE_CXXFLAGS += /wd4996 /Qpar /Gy /Gw /FS
		COMPILER_NAME = VC12
		CONFIG += c++11
	}
	win32-msvc2015{
		QMAKE_CXXFLAGS += /wd4996 /Qpar /Gy /Gw /FS
		COMPILER_NAME = VC14
		CONFIG += c++11
	}

	contains(QMAKE_HOST.arch, x86_64){
		PLATFORM_NAME = x64
		# SSE2 enabled by default for x86_64
	}
	else{
		PLATFORM_NAME = Win32
		QMAKE_CXXFLAGS += /arch:SSE2
	}
}

*-clang* | *-llvm*{
	QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-variable
	QMAKE_CXXFLAGS_WARN_ON += -Wno-overloaded-virtual
	QMAKE_CXXFLAGS_WARN_ON += -Wno-format-security
}

*-gcc*{
	QMAKE_CXXFLAGS_WARN_ON += -wformat-security
	QMAKE_CXXFLAGS += /openmp
	LIBS += -lgomp
}

CONFIGURATION_NAME = Unknown

CONFIG(debug, debug|release){
	CONFIGURATION_NAME = Debug
}

CONFIG(release, debug|release){
	CONFIGURATION_NAME = Release
}

COMPILER_CODE = $$COMPILER_NAME
contains(QMAKE_HOST.arch, x86_64){
	COMPILER_CODE = $$COMPILER_NAME"_64"
}


COMPILER_DIR = $$CONFIGURATION_NAME$$COMPILER_CODE

AUXINCLUDEDIR = AuxInclude/Qt$${QT_MAJOR_VERSION}_$$COMPILER_CODE
AUXINCLUDEPATH = ../../../$$AUXINCLUDEDIR


!win32-msvc*{
	QMAKE_LFLAGS -= -mthreads
	QMAKE_CXXFLAGS += -fno-threadsafe-statics
}

win32-msvc*{
	# activate debug info also for release builds
	QMAKE_LFLAGS += /DEBUG
}

# path definition
INCLUDEPATH += $$AUXINCLUDEPATH
UI_DIR = $$AUXINCLUDEPATH/GeneratedFiles/"$$TARGET"
MOC_DIR = $$AUXINCLUDEPATH/GeneratedFiles/"$$TARGET"
ACF_TRANSLATIONS_OUTDIR = $$AUXINCLUDEPATH/GeneratedFiles/"$$TARGET"

# objects directory
OBJECTS_DIR = $$AUXINCLUDEPATH/GeneratedFiles/$$CONFIGURATION_NAME/"$$TARGET"

INCLUDEPATH += $$PWD/../../Include
INCLUDEPATH += $$PWD/../../$$AUXINCLUDEDIR

