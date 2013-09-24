import qbs.base 1.0
// Standard settings for an ACF static library

StaticLibrary{
	type: ["staticlibrary", "acf_share"]

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
		condition: acf.installIncludes == true
		fileTagsFilter: ["hpp"]
		qbs.install: true
		qbs.installDir: "Include/GeneratedFiles/" + product.name
	}

	Group{
		condition: acf.installProject === undefined || acf.installProject == acf.projectName
		fileTagsFilter: ["staticlibrary"]
		qbs.install: true
		qbs.installDir: acf.targetLibSubdir
	}
}
