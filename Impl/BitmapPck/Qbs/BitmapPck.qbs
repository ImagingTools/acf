import qbs.base 1.0
import AcfPackage as AcfPackage

AcfPackage{
	name: "BitmapPck"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "iimg" }
	Export{
		Depends{ name: "iimg" }
	}
}
