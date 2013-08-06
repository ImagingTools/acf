import qbs.base 1.0
// Standard settings for an ACF application

import "GeneralProduct.qbs" as GeneralProduct

GeneralProduct{
	type: "application"

	cpp.includePaths: ["../..", "../../../Include"]

	files: ["../*.h", "../*.cpp"]

	destinationDirectory: "Bin"
}
