#General base configuration should be included from all ACF-based projects

CONFIG += stl
CONFIG -= exceptions
CONFIG -= silent
CONFIG += rtti

PLATFORM_CODE = i86
win32:contains(QMAKE_HOST.arch, x86_64) | *-64{
	PLATFORM_CODE = x64
}

CONFIG(release, debug|release){
	#sse support
	CONFIG += sse2
}

COMPILER_NAME = QMake

CONFIG += depend_includepath

win32-msvc*{
	COMPILER_NAME = VC
	QMAKE_CXXFLAGS += /wd4127 /wd4250 /wd4347 /wd4355 /wd4365 /wd4481 /wd4505 /wd4510 /wd4511 /wd4512 /wd4548 /wd4571 /wd4619 /wd4625 /wd4626 /wd4640 /wd4702 /wd4710 /wd4820 /wd4826
	QMAKE_CXXFLAGS_WARN_ON = -W4
	QMAKE_CXXFLAGS += /MP /fp:fast
	QMAKE_CXXFLAGS += -D_SCL_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_WARNINGS
	QMAKE_CXXFLAGS += /bigobj

	# Qt-BUG 31516. Remove it after the fix in Qt!!!
	QMAKE_CXXFLAGS += /wd4718

	contains(QMAKE_HOST.arch, x86_64){
		PLATFORM_CODE = x64
		# SSE2 enabled by default for x86_64
	}
	else{
		PLATFORM_CODE = Win32
		QMAKE_CXXFLAGS += /arch:SSE2
	}
}

!isEmpty(MSVC_VER){
	contains(MSVC_VER, "8.0"){
		COMPILER_NAME = VC8

		message("Using Visual Studio 2005");
	}

	contains(MSVC_VER, "9.0"){
		COMPILER_NAME = VC9
		QMAKE_CXXFLAGS -= /Gd
		QMAKE_CXXFLAGS -= /GD
		QMAKE_CXXFLAGS -= -Gd
		QMAKE_CXXFLAGS -= -GD

		message("Using Visual Studio 2008");
	}

	contains(MSVC_VER, "10.0"){
		QMAKE_CXXFLAGS += /wd4996
		COMPILER_NAME = VC10

		message("Using Visual Studio 2010");
	}

	contains(MSVC_VER, "11.0"){
		QMAKE_CXXFLAGS += /wd4996
		COMPILER_NAME = VC11

		message("Using Visual Studio 2012");
	}

	contains(MSVC_VER, "12.0"){
		QMAKE_CXXFLAGS += /wd4996 /Qpar /Gy /Gw /FS
		COMPILER_NAME = VC12
		CONFIG += c++11

		message("Using Visual Studio 2013");
	}

	contains(MSVC_VER, "14.0"){
		QMAKE_CXXFLAGS += /Qpar /Gy /Gw /FS /Zc:threadSafeInit-
		COMPILER_NAME = VC14

		CONFIG(release, debug|release){
			#extra optimizations
			QMAKE_CXXFLAGS += /Ot /Oi /Ob2 /GS-
		}

		win32:contains(QMAKE_HOST.arch, x86_64) | *-64{
			QMAKE_LFLAGS += /MACHINE:X64
		}

		message("Using Visual Studio 2015");
	}

	contains(MSVC_VER, "15.0"){
		QMAKE_CXXFLAGS += /Qpar /Gy /Gw /FS /Zc:threadSafeInit- /D__STDC_LIMIT_MACROS
		COMPILER_NAME = VC15

		CONFIG(release, debug|release){
			#extra optimizations
			QMAKE_CXXFLAGS += /Ot /Oi /Ob2 /GS-
		}

		win32:contains(QMAKE_HOST.arch, x86_64) | *-64{
			QMAKE_LFLAGS += /MACHINE:X64
		}

		message("Using Visual Studio 2017");
	}

	contains(MSVC_VER, "16.0"){
		QMAKE_CXXFLAGS += /Qpar /Gy /Gw /FS /Zc:threadSafeInit- /D__STDC_LIMIT_MACROS
                COMPILER_NAME = VC16

		CONFIG(release, debug|release){
			#extra optimizations
			QMAKE_CXXFLAGS += /Ot /Oi /Ob2 /GS-
		}

		win32:contains(QMAKE_HOST.arch, x86_64) | *-64{
			QMAKE_LFLAGS += /MACHINE:X64
		}

		message("Using Visual Studio 2019");
	}
}
else{
	win32-msvc2005{
		COMPILER_NAME = VC8

		message("Using Visual Studio 2005");
	}

	win32-msvc2008{
		COMPILER_NAME = VC9
		QMAKE_CXXFLAGS -= /Gd
		QMAKE_CXXFLAGS -= /GD
		QMAKE_CXXFLAGS -= -Gd
		QMAKE_CXXFLAGS -= -GD

		message("Using Visual Studio 2008");
	}

	win32-msvc2010{
		QMAKE_CXXFLAGS += /wd4996
		COMPILER_NAME = VC10

		message("Using Visual Studio 2010");
	}

	win32-msvc2012{
		QMAKE_CXXFLAGS += /wd4996
		COMPILER_NAME = VC11

		message("Using Visual Studio 2012");
	}

	win32-msvc2013{
		QMAKE_CXXFLAGS += /wd4996 /Qpar /Gy /Gw /FS
		COMPILER_NAME = VC12
		CONFIG += c++11

		message("Using Visual Studio 2013");
	}
}

*-icc*{
	COMPILER_NAME = ICC
	CONFIG += c++11
	MAKE_CXXFLAGS += /arch:SSE3
}

*-clang* | *-llvm*{
	CONFIG += c++11
	COMPILER_NAME = Clang
	QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-variable
	QMAKE_CXXFLAGS_WARN_ON += -Wno-overloaded-virtual
	QMAKE_CXXFLAGS_WARN_ON += -Wno-format-security
}

*-gcc*{
	QMAKE_CXXFLAGS_WARN_ON += -wformat-security
	QMAKE_CXXFLAGS += /openmp
	LIBS += -lgomp
	COMPILER_NAME = GCC
}

*-g++*{
	QMAKE_CXXFLAGS += -fpermissive
	QMAKE_CFLAGS_RELEASE = -Ofast
	COMPILER_NAME = GCC
}

# MinGW compiler used under Windows instead of GCC
win32{
	*-gcc* | *-g++*{
		contains(QMAKE_HOST.arch, x86_64){
			COMPILER_NAME = MinGW_64
		}
		else{
			COMPILER_NAME = MinGW
		}
	}
}

CONFIG(debug, debug|release){
	CONFIGURATION_NAME = Debug
}

CONFIG(release, debug|release){
	CONFIGURATION_NAME = Release
}

COMPILER_CODE = $$COMPILER_NAME
contains(QMAKE_HOST.arch, x86_64){
	!macx-clang-32{
		COMPILER_CODE = $$COMPILER_NAME"_64"
	}
}


COMPILER_DIR = $$CONFIGURATION_NAME$$COMPILER_CODE

AUXINCLUDEDIR = AuxInclude/Qt$${QT_MAJOR_VERSION}_$$COMPILER_CODE
AUXINCLUDEPATH = ../../../$$AUXINCLUDEDIR


!win32-msvc*{
	QMAKE_LFLAGS -= -mthreads
	QMAKE_CXXFLAGS += -fno-threadsafe-statics
}

# path definition
INCLUDEPATH += $$AUXINCLUDEPATH
UI_DIR = $$AUXINCLUDEPATH/GeneratedFiles/"$$TARGET"
MOC_DIR = $$AUXINCLUDEPATH/GeneratedFiles/"$$TARGET"
RCC_DIR = $$AUXINCLUDEPATH/GeneratedFiles/"$$TARGET"
ACF_TRANSLATIONS_OUTDIR = $$AUXINCLUDEPATH/GeneratedFiles/"$$TARGET"

# objects directory
OBJECTS_DIR = $$AUXINCLUDEPATH/GeneratedFiles/$$CONFIGURATION_NAME/"$$TARGET"

INCLUDEPATH += $$PWD/../../Include
INCLUDEPATH += $$PWD/../../$$AUXINCLUDEDIR

# Get build output directory of shadow build:
ACFDIRBUILD = $$(ACFDIR_BUILD)

# Add build output directory to include path
!isEmpty( ACFDIRBUILD ){
	INCLUDEPATH += $$(ACFDIR_BUILD)/$$AUXINCLUDEDIR
}


