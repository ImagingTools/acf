import qbs.base 1.0
import AcfLibrary as AcfLibrary

AcfLibrary{
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
