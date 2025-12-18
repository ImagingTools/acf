import qbs.base 1.0
import AcfLibrary as AcfLibrary

AcfLibrary{
	files: ["../*.h", "../*.cpp"]

	Depends{ name: "i2d" }
	Depends{ name: "icomp" }

	Export{
		Depends{ name: "i2d" }
		Depends{ name: "icomp" }
	}
}
