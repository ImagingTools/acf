import qbs.base 1.0
import AcfPackage as AcfPackage

AcfPackage{
	name: "FilePck"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "ifile" }
	Depends{ name: "ilog" }
	Export{
		Depends{ name: "ifile" }
		Depends{ name: "ilog" }
	}
}
