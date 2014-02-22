import qbs.base 1.0
import AcfPackage as AcfPackage

AcfPackage{
	name: "FileGuiPck"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "icomp" }
	Depends{ name: "iqtgui" }
	Depends{ name: "ifilegui" }

	Export{
		Depends{ name: "icomp" }
		Depends{ name: "iqtgui" }
		Depends{ name: "ifilegui" }
	}
}
