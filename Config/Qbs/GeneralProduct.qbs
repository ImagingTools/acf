import qbs.base 1.0
// General base configuration should be included from all ACF-based projects

Product{
	Depends{ name: "Qt.core" }
	Depends{ name: "cpp" }

	Export{
		Depends{ name: "cpp" }
		Depends{ name: "Qt.core" }
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

	cpp.defines: ['I_QBS']

	property string COMPILER_DIR
	Properties{
		condition: cpp.debugInformation == true
		COMPILER_DIR: "Debug" + COMPILER_NAME
	}
	Properties{
		condition: cpp.debugInformation == false
		COMPILER_DIR: "Release" + COMPILER_NAME
	}
}
