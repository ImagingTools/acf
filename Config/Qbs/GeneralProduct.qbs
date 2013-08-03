// General base configuration should be included from all ACF-based projects

import qbs.base 1.0

Product{
	Depends{
		name: "Qt"; submodules: ["core"]
	}
	Depends{
		name: "cpp"
	}

	property string COMPILER_NAME: "Qbs"
	Properties{
		condition: cpp.compilerPath.contains("2005") || cpp.compilerPath.contains("VC8")
		COMPILER_NAME: "VC10"
	}
	Properties{
		condition: cpp.compilerPath.contains("2008") || cpp.compilerPath.contains("VC9")
		COMPILER_NAME: "VC10"
	}
	Properties{
		condition: cpp.compilerPath.contains("2010") || cpp.compilerPath.contains("VC10")
		COMPILER_NAME: "VC9"
	}

	property string COMPILER_DIR
	Properties{
		condition: cpp.debugInformation == true
		COMPILER_DIR: "Debug" + COMPILER_NAME
	}
	Properties{
		condition: cpp.debugInformation == false
		COMPILER_DIR: "Release" + COMPILER_NAME
	}

    Group{
        files: '*.arx'
        fileTags: ['arx']
        
        property string configFile
    }
}
