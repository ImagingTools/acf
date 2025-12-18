import qbs.base 1.0
import AcfPackage as AcfPackage

AcfPackage{
	files: ["../*.h", "../*.cpp"]

	Depends{ name: "ilog" }
	Export{
		Depends{ name: "ilog" }
	}
}
