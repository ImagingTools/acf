import qbs.base 1.0
// Standard settings for an ACF package project

DynamicLibrary{
	type: ["dynamiclibrary", "acf_share", "acfComponent"]

	Depends{ name: "ipackage" }
	Depends{ name: "acf" }
	Depends{ name: "Qt.core" }
	Depends{ name: "cpp" }

	acf.projectRoot: path + "/../../.."

	Properties{
		condition: qbs.toolchain.contains("gcc") || qbs.toolchain.contains("clang")
		cpp.cxxFlags: ['-Wno-format-security', '-Wno-unknown-pragmas']
	}

	cpp.defines: ['I_QBS']
	cpp.includePaths: ["../.."]
	cpp.dynamicLibraryPrefix: ""
	cpp.dynamicLibrarySuffix: ".arp"

	Export{
		Depends{ name: "Qt.core" }
		Depends{ name: "ipackage" }
		Depends{ name: "cpp" }

		cpp.defines: ['I_QBS']
		cpp.includePaths: ["../.."]
	}

	Group{
		condition: (acf.installProject === undefined || acf.installProject == acf.projectName) && (acf.targetPckSubdir !== undefined)
		fileTagsFilter: ["dynamiclibrary"]
		qbs.install: true
		qbs.installDir: acf.targetPckSubdir
	}
}
