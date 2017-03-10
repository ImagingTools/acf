import qbs.base 1.0
import "../modules/acf/AcfService.js" as AcfService

// Standard settings for an ACF application

CppApplication{
	type: ["application", "acf_share"]

	consoleApplication: true

	Depends{ name: "cpp" }
	Depends{ name: "acf" }
	Depends{ name: "Qt.core" }

	Properties{
		condition: qbs.toolchain.contains("gcc") || qbs.toolchain.contains("clang")
		cpp.cxxFlags: ['-Wno-format-security', '-Wno-unknown-pragmas']
	}

	cpp.defines: ['I_QBS']

	Group{
		condition: (project.acfInstallSubDir !== undefined)
		fileTagsFilter: ["application"]
		qbs.install: true
		qbs.installDir: AcfService.getTargetPath(product, project, acf.targetBinSubdir)
	}
}
