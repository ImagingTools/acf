import qbs.base 1.0
import AcfLibrary as AcfLibrary

AcfLibrary{
	name: "iqtprm"

	files: ["../*.h", "../*.cpp", "../*.ui"]

	Depends{ name: "Qt.gui" }
	Depends{ name: "Qt.widgets" }
	Depends{ name: "iprm" }
	Depends{ name: "iqtgui" }
	Depends{ name: "ifilegui" }

	Export{
		Depends{ name: "Qt.gui" }
		Depends{ name: "Qt.widgets" }
		Depends{ name: "iprm" }
		Depends{ name: "iqtgui" }
		Depends{ name: "ifilegui" }
	}
}
