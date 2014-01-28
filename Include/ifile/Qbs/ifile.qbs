import qbs.base 1.0
import AcfStaticLibrary as AcfStaticLibrary

AcfStaticLibrary{
	name: "ifile"

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
