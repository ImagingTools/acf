import qbs.base 1.0
import AcfStaticLibrary as AcfStaticLibrary

AcfStaticLibrary{
	name: "DocView"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "icomp" }
	Depends{ name: "iqtgui" }
	Depends{ name: "idoc" }
	Depends{ name: "Qt.widgets" }
	Depends{ name: "Qt.gui" }
	Depends{ name: "Qt.printsupport" }

	Export{
		Depends{ name: "icomp" }
		Depends{ name: "iqtgui" }
		Depends{ name: "idoc" }
		Depends{ name: "Qt.widgets" }
		Depends{ name: "Qt.gui" }
		Depends{ name: "Qt.printsupport" }
	}
}
