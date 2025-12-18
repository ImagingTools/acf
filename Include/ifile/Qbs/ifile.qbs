import qbs.base 1.0
import AcfLibrary as AcfLibrary

AcfLibrary{
	files: ["../*.h", "../*.cpp"]

	Depends{ name: "ilog" }
	Depends{ name: "Qt.widgets" }	// TODO: remove this dependency
	Depends{ name: "Qt.gui" }	// TODO: remove this dependency

	Export{
		Depends{ name: "ibase" }
		Depends{ name: "ilog" }
		Depends{ name: "Qt.widgets" }	// TODO: remove this dependency
		Depends{ name: "Qt.gui" }	// TODO: remove this dependency
	}
}
