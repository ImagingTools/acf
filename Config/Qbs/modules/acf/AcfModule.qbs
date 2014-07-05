import qbs.base 1.0

Module{
    condition: false

	Depends{ name: "cpp" }

	property string compilerName
	property string compileMode

	Properties{
		condition: (qbs.targetOS.contains("linux") || qbs.targetOS.contains("unix")) && !qbs.targetOS.contains("osx")
		compilerName: "QMake"
	}
	Properties{
		condition: qbs.targetOS.contains("osx")
		compilerName: "OSX"
	}
	Properties{
		condition: qbs.toolchain.contains("mingw")
		compilerName: "MinGW"
	}
	Properties{
		condition: qbs.toolchain.contains("msvc") && (cpp.compilerPath.search(/Studio 8/i) >= 0 || cpp.compilerPath.search(/2005/i) >= 0 || cpp.toolchainInstallPath.search(/Studio 8/i) >= 0)
		compilerName: "VC8"
	}
	Properties{
		condition: qbs.toolchain.contains("msvc") && (cpp.compilerPath.search(/Studio 9/i) >= 0 || cpp.compilerPath.search(/2008/i) >= 0 || cpp.toolchainInstallPath.search(/Studio 9/i) >= 0)
		compilerName: "VC9"
	}
	Properties{
		condition: qbs.toolchain.contains("msvc") && (cpp.compilerPath.search(/Studio 10/i) >= 0 || cpp.compilerPath.search(/2010/i) >= 0 || cpp.toolchainInstallPath.search(/Studio 10/i) >= 0)
		compilerName: "VC10"
	}
	Properties{
		condition: qbs.toolchain.contains("msvc") && (cpp.compilerPath.search(/Studio 11/i) >= 0 || cpp.compilerPath.search(/2012/i) >= 0 || cpp.toolchainInstallPath.search(/Studio 11/i) >= 0)
		compilerName: "VC11"
	}
	Properties{
		condition: qbs.toolchain.contains("msvc") && (cpp.compilerPath.search(/Studio 12/i) >= 0 || cpp.compilerPath.search(/2013/i) >= 0 || cpp.toolchainInstallPath.search(/Studio 12/i) >= 0)
		compilerName: "VC12"
	}

	Properties{
		condition: qbs.debugInformation == true
		compileMode: "Debug"
	}
	Properties{
		condition: qbs.debugInformation == false
		compileMode: "Release"
	}

	property bool installIncludes: false							// if enabled the includes will be installed
	property bool installProjectSubdirs: false							// if enabled the installed files will be placed in project subdirs
	property string compilerDir: compileMode + compilerName
	property string compilerDirExt: (Qt.core.versionMajor > 4)? compilerDir + "_Qt" + Qt.core.versionMajor: compilerDir
	property string targetBinSubdir: ""
	property string targetPckSubdir
	property string targetLibSubdir
	property string generatedOutputDir								// Path where stuff will be generated, if undefined "GeneratedPath/" + product.name will be taken
	property path acfConfigurationFile								// ACF configuration file ARX compiler
	property path trConfigurationFile: acfConfigurationFile			// ACF configuration file for xtracf transformations
	property path trRegFile											// ACF registry file for xtracf transformations
	property stringList trOutputType								// ACF transformation output tags
	property stringList xpcPackageDirs								// Extra directories placed into generated XPC file

	FileTagger{
		patterns: ["*.arx"]
		fileTags: ["arx"]
	}

	FileTagger{
		patterns: ["*.xtracf"]
		fileTags: ["xtracf"]
	}

	FileTagger{
		patterns: ["*.arp"]
		fileTags: ["acfComponent"]
	}

	FileTagger{
		patterns: ["*.xpc"]
		fileTags: ["xpc_file"]
	}
}
