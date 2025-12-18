import qbs.base 1.0
import AcfLibrary as AcfLibrary

AcfLibrary{
	files: ["../*.h", "../*.cpp"]

	Depends{ name: "istd" }
	Depends{ name: "Qt.test" }

	Export{
		Depends{ name: "istd" }
		Depends{ name: "Qt.test" }
	}
}
