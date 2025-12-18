import qbs.base 1.0
import AcfLibrary as AcfLibrary

AcfLibrary{
	files: ["../*.h", "../*.cpp"]

	Depends{ name: "ifile" }
	Depends{ name: "iprm" }
	Depends{ name: "iqt" }

	Export{
		Depends{ name: "ilog" }
		Depends{ name: "iprm" }
		Depends{ name: "iqt" }
	}
}
