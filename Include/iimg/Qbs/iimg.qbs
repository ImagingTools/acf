import qbs.base 1.0
import AcfStaticLibrary as AcfStaticLibrary

AcfStaticLibrary{
	name: "iimg"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "idoc" }
	Depends{ name: "i2d" }
	Depends{ name: "iqt" }
	Depends{ name: "icmm" }
	Depends{ name: "ibase" }
	Depends{ name: "icomp" }
	Depends{ name: "Qt.gui" }

	Export{
		Depends{ name: "idoc" }
		Depends{ name: "i2d" }
		Depends{ name: "iqt" }
		Depends{ name: "icmm" }
		Depends{ name: "ibase" }
		Depends{ name: "icomp" }
		Depends{ name: "Qt.gui" }
	}
}
