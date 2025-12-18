import qbs.base 1.0
import AcfLibrary as AcfLibrary
//import "../../../Config/Qbs/QtRules.qbs" as QtRules

AcfLibrary{
	files: ["../*.h", "../*.cpp", "../*.ui", "../*.qrc"]

	Depends{ name: "iqt" }
	Depends{ name: "Qt.gui" }
	Depends{ name: "Qt.widgets" }

	Export{
		Depends{ name: "iqt" }
		Depends{ name: "Qt.gui" }
		Depends{ name: "Qt.widgets" }
	}
}
