import qbs.base 1.0
// Standard settings for an ACF static library

StaticLibrary{
	type: ["staticlibrary", "acf_share"]

	destinationDirectory: "Lib"

	Depends{ name: "acf" }
	Depends{ name: "cpp" }
	Depends{ name: "Qt.core" }

	cpp.defines: ['I_QBS']
	cpp.includePaths: ["../..", product.buildDirectory]

	Export{
		Depends{ name: "Qt.core" }
		Depends{ name: "cpp" }

		cpp.defines: ['I_QBS']
		cpp.includePaths: ["../..", product.buildDirectory]
	}
}
