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

COMPILER_NAME = QMake
PLATFORM_NAME = Unix

win32-msvc*{
	COMPILER_NAME = VC
	PLATFORM_NAME = Win
	QMAKE_CXXFLAGS += /wd4127 /wd4250 /wd4347 /wd4355 /wd4365 /wd4505 /wd4510 /wd4511 /wd4512 /wd4548 /wd4571 /wd4619 /wd4625 /wd4626 /wd4640 /wd4702 /wd4710 /wd4820 /wd4826
	QMAKE_CXXFLAGS_WARN_ON = -W4
    QMAKE_CXXFLAGS += /MP /openmp- /fp:fast /arch:SSE2

	win32-msvc2005{
		COMPILER_NAME = VC8
	}
	win32-msvc2008{
		COMPILER_NAME = VC9
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
    	QMAKE_CXXFLAGS += /wd4996 /Qpar /Gy /Gw
		QMAKE_LFLAGS += /LTCG:PGO
		COMPILER_NAME = VC12
	}

	contains(QMAKE_HOST.arch, x86_64){
		PLATFORM_NAME = Win64
	}
	else{
		PLATFORM_NAME = Win32
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

LIBRARY_SUFIX =

CONFIG(debug, debug|release){
	COMPILER_DIR = Debug$$COMPILER_NAME
	LIBRARY_SUFIX = d
}
CONFIG(release, debug|release){
	COMPILER_DIR = Release$$COMPILER_NAME
	CONFIG += sse2
}

!win32-msvc*{
	QMAKE_LFLAGS -= -mthreads
	QMAKE_CXXFLAGS += -fno-threadsafe-statics
}

INCLUDEPATH += $$PWD/../../Include
