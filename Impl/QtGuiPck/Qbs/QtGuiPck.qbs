import qbs.base 1.0
import AcfPackage as AcfPackage

AcfPackage{
	name: "QtGuiPck"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "icomp" }
	Depends{ name: "iqtgui" }
	Depends{ name: "ifilegui" }
	Depends{ name: "iqt2d" }
	Depends{ name: "iqtprm" }
	Depends{ name: "iqtdoc" }
	Depends{ name: "iloggui" }

	Export{
		Depends{ name: "icomp" }
		Depends{ name: "iqtgui" }
		Depends{ name: "ifilegui" }
		Depends{ name: "iqt2d" }
		Depends{ name: "iqtprm" }
		Depends{ name: "iqtdoc" }
		Depends{ name: "iloggui" }
	}
}
