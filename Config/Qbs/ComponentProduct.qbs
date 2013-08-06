import qbs.base 1.0
// Standard settings for an ACF package project

import "GeneralProduct.qbs" as GeneralProduct

GeneralProduct{
	type: "dynamiclibrary"

	destinationDirectory: "Bin"

	cpp.includePaths: "../.."

	cpp.dynamicLibraryPrefix: ""
	cpp.dynamicLibrarySuffix: ".arp"

	Export{
		cpp.includePaths: "../.."
	}
}
