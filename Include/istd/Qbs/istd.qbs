import qbs.base 1.0
import AcfStaticLibrary as AcfStaticLibrary

AcfStaticLibrary{
	name: "istd"

	files: ["../*.h", "../*.cpp"]
}
