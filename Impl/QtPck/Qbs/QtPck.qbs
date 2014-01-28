import qbs.base 1.0
import AcfPackage as AcfPackage

AcfPackage{
	name: "QtPck"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "iqt" }
	Depends{ name: "iimg" }
	Depends{ name: "ifile" }
	Depends{ name: "iqtdoc" }

	Export{
		Depends{ name: "iqt" }
		Depends{ name: "iimg" }
		Depends{ name: "ifile" }
		Depends{ name: "iqtdoc" }
	}
}
