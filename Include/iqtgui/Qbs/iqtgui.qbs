import qbs.base 1.0
import AcfStaticLibrary as AcfStaticLibrary
//import "../../../Config/Qbs/QtRules.qbs" as QtRules

AcfStaticLibrary{
	name: "iqtgui"

	files: ["../*.h", "../*.cpp", "../*.ui", "../*.qrc"]

	Depends{ name: "iqt" }
	Depends{ name: "iwidgets" }
	Depends{ name: "iprm" }
	Depends{ name: "ibase" }
	Depends{ name: "icomp" }
	Depends{ name: "Qt.gui" }
	Depends{ name: "Qt.widgets" }

	Export{
		Depends{ name: "iqt" }
		Depends{ name: "iwidgets" }
		Depends{ name: "iprm" }
		Depends{ name: "ibase" }
		Depends{ name: "icomp" }
		Depends{ name: "Qt.gui" }
		Depends{ name: "Qt.widgets" }
	}
}
