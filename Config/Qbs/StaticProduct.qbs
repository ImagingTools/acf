import qbs.base 1.0
// Standard settings for an ACF static library

StaticLibrary{
	type: ["staticlibrary", "acf_share"]

	destinationDirectory: acf.libSubdir

	Depends{ name: "acf" }
	Depends{ name: "cpp" }
	Depends{ name: "Qt.core" }

	acf.projectRoot: path + "/../../.."

	cpp.defines: ['I_QBS']
	cpp.includePaths: ["../..", product.buildDirectory]

	Export{
		Depends{ name: "Qt.core" }
		Depends{ name: "cpp" }

		cpp.defines: ['I_QBS']
		cpp.includePaths: ["../..", product.buildDirectory]
	}

	Group{
		fileTagsFilter: ["staticlibrary"]
		qbs.install: true
		qbs.installDir: acf.libSubdir
	}
}
