import qbs.base 1.0
// Standard settings for an ACF package project

DynamicLibrary{
	destinationDirectory: "Bin"

	type: ["dynamiclibrary", "acfComponent"]

	Depends{ name: "Qt.core" }
	Depends{ name: "cpp" }
	Depends{ name: "ipackage" }

	cpp.defines: ['I_QBS']

	cpp.dynamicLibraryPrefix: ""
	cpp.dynamicLibrarySuffix: ".arp"

	Export{
		Depends{ name: "cpp" }
		Depends{ name: "Qt.core" }
		Depends{ name: "ipackage" }

		cpp.defines: ['I_QBS']
		cpp.includePaths: ["../.."]
	}
}
