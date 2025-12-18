import qbs.base 1.0
import AcfLibrary as AcfLibrary

AcfLibrary{
	files: ["../*.h", "../*.cpp", "../*.ui", "../*.qrc"]

	Depends{ name: "iview" }
	Depends{ name: "i2d" }
	Depends{ name: "Qt.widgets" }
	Depends{ name: "Qt.gui" }

	Export{
		Depends{ name: "iview" }
		Depends{ name: "i2d" }
		Depends{ name: "Qt.widgets" }
		Depends{ name: "Qt.gui" }
	}
}
