import qbs.base 1.0
import AcfLibrary as AcfLibrary

AcfLibrary{
	files: ["../*.h", "../*.cpp"]

	Depends{ name: "iser" }

	Export{
		Depends{ name: "iser" }
	}
}
