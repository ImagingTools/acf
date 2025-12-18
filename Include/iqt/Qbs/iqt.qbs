import qbs.base 1.0
import AcfLibrary as AcfLibrary

AcfLibrary{
	files: ["../*.h", "../*.cpp"]

	Depends{ name: "Qt.xml" }
	Depends{ name: "Qt.network" }
	Depends{ name: "i2d" }
	Depends{ name: "icomp" }
	Depends{ name: "ilog" }
	Depends{ name: "ifile" }

	Export{
		Depends{ name: "Qt.xml" }
		Depends{ name: "Qt.network" }
		Depends{ name: "i2d" }
		Depends{ name: "icomp" }
		Depends{ name: "ilog" }
		Depends{ name: "ifile" }
	}
}
