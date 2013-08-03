// Standard settings for an ACF static library

import qbs.base 1.0
import "GeneralProduct.qbs" as GeneralProduct

GeneralProduct{
	type: "staticlibrary"

	cpp.includePaths: ['../..']

	files: ["../*.h", "../*.cpp", "../*.qrc", "../*.ui"]

	destinationDirectory: "Lib"

	Export{
		Depends{
			name: "cpp"
		}
		cpp.includePaths: "../.."
	}
}
