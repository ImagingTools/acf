import qbs.base 1.0
import "../modules/acf/AcfService.js" as AcfService

// Standard settings for an ACF package project

DynamicLibrary{
	type: ["dynamiclibrary", "acf_share", "acfComponent"]

	Depends{ name: "cpp" }
	Depends{ name: "Qt.core" }
	Depends{ name: "acf" }
	Depends{ name: "ipackage" }

	Properties{
		condition: qbs.toolchain.contains("gcc") || qbs.toolchain.contains("clang")
		cpp.cxxFlags: ['-Wno-format-security', '-Wno-unknown-pragmas']
	}

	cpp.defines: ['I_QBS']
	cpp.includePaths: ["../.."]
	cpp.dynamicLibraryPrefix: ""
	cpp.dynamicLibrarySuffix: ".arp"

	Export{
		cpp.includePaths: ["../.."]
	}

	Group{
		condition: (acf.targetPckSubdir !== undefined) && (project.acfInstallSubDir !== undefined)
		fileTagsFilter: ["dynamiclibrary"]
		qbs.install: true
		qbs.installDir: AcfService.getTargetPath(product, project, acf.targetPckSubdir)
	}
}
