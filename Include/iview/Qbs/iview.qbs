import qbs.base 1.0
import AcfLibrary as AcfLibrary

AcfLibrary{
	files: ["../*.h", "../*.cpp", "../*.ui"]

	Depends{ name: "iqtgui" }
	Depends{ name: "iimg" }
	Depends{ name: "Qt.gui" }
	Depends{ name: "Qt.widgets" }

	Export{
		Depends{ name: "iqtgui" }
		Depends{ name: "iimg" }
		Depends{ name: "Qt.gui" }
		Depends{ name: "Qt.widgets" }
	}
}
