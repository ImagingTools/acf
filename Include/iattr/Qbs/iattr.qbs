import qbs.base 1.0
import AcfStaticLibrary as AcfStaticLibrary

AcfStaticLibrary{
	name: "iattr"

	files: ["../*.h", "../*.cpp", "../*.ui"]

	Depends{ name: "iser" }

	Export{
		Depends{ name: "iser" }
	}
}
