import qbs.base 1.0
// Standard settings for an ACF package project

DynamicLibrary{
	type: ["dynamiclibrary", "acf_share"]

	destinationDirectory: "Bin"

	Depends{ name: "ipackage" }
	Depends{ name: "acf" }
	Depends{ name: "Qt.core" }
	Depends{ name: "cpp" }

	cpp.defines: ['I_QBS']

	cpp.dynamicLibraryPrefix: ""
	cpp.dynamicLibrarySuffix: ".arp"

	Export{
		Depends{ name: "Qt.core" }
		Depends{ name: "ipackage" }
		Depends{ name: "cpp" }

		cpp.defines: ['I_QBS']
		cpp.includePaths: ["../.."]
	}
}
