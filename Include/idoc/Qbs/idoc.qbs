import qbs.base 1.0
import AcfLibrary as AcfLibrary

AcfLibrary{
	files: ["../*.h", "../*.cpp"]

	Depends{ name: "imod" }
	Depends{ name: "iser" }
	Depends{ name: "icomp" }
	Depends{ name: "ifile" }

	Export{
		Depends{ name: "imod" }
		Depends{ name: "iser" }
		Depends{ name: "icomp" }
		Depends{ name: "ifile" }
	}
}
