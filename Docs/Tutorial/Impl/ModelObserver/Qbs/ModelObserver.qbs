import qbs.base 1.0
import AcfStaticLibrary as AcfStaticLibrary

AcfStaticLibrary{
	name: "ModelObserver"

	files: ["../*.h", "../*.cpp", "../*.ui"]

	Depends{ name: "icomp" }
	Depends{ name: "iqtgui" }
	Depends{ name: "Qt.widgets" }
	Depends{ name: "Qt.gui" }

	Export{
		Depends{ name: "icomp" }
		Depends{ name: "iqtgui" }
		Depends{ name: "Qt.widgets" }
		Depends{ name: "Qt.gui" }
	}
}
