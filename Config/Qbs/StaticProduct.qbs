import qbs.base 1.0
// Standard settings for an ACF static library

import "GeneralProduct.qbs" as GeneralProduct

GeneralProduct{
	type: "staticlibrary"

	destinationDirectory: "Lib"

	cpp.includePaths: "../.."

	Export{
		cpp.includePaths: "../.."
	}
}
