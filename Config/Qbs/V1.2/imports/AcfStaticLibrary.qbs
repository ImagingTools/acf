import qbs.base 1.0
import "../modules/acf/AcfService.js" as AcfService

// Standard settings for an ACF static library

StaticLibrary{
	type: ["staticlibrary", "acf_share"]

	Depends{ name: "acf" }
	Depends{ name: "cpp" }
	Depends{ name: "Qt.core" }

	Properties{
		condition: qbs.toolchain.contains("gcc") || qbs.toolchain.contains("clang")
		cpp.cxxFlags: ['-Wno-format-security', '-Wno-unknown-pragmas']
	}

	cpp.defines: ['I_QBS']
	cpp.includePaths: ["../..", product.buildDirectory]

	Export{
		Depends{ name: "Qt.core" }
		Depends{ name: "cpp" }

		cpp.defines: ['I_QBS']
		cpp.includePaths: ["../..", product.buildDirectory]
	}

	Group{
		condition: (acf.targetGeneratedSubdir !== undefined) && (project.acfInstallSubDir !== undefined)
		fileTagsFilter: ["hpp"]
		qbs.install: true
		qbs.installDir: AcfService.getTargetPath(product, project, acf.targetGeneratedSubdir + "/GeneratedFiles/" + product.name)
	}

	Group{
		condition: (acf.targetLibSubdir !== undefined) && (project.acfInstallSubDir !== undefined)
		fileTagsFilter: ["staticlibrary"]
		qbs.install: true
		qbs.installDir: AcfService.getTargetPath(product, project, acf.targetLibSubdir)
	}
}
