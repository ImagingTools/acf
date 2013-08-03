// Standard settings for an ACF package project

import qbs.base 1.0
import "GeneralProduct.qbs" as GeneralProduct

GeneralProduct{
	type: "dynamiclibrary"

	cpp.includePaths: ["../..", "../../../Include"]

	files: ["../*.h", "../*.cpp"]

	destinationDirectory: "Bin"

	dynamicLibraryPrefix: ""
	dynamicLibrarySuffix: ".arp"
}
