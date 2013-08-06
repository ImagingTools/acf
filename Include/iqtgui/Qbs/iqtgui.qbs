import qbs.base 1.0
import "../../../Config/Qbs/StaticProduct.qbs" as StaticProduct
//import "../../../Config/Qbs/QtRules.qbs" as QtRules

StaticProduct{
	name: "iqtgui"

	files: ["../*.h", "../*.cpp", "../*.ui", "../*.qrc"]

	Depends{ name: "Qt.gui" }
	Depends{ name: "Qt.widgets" }

	Export{
		Depends{ name: "icomp" }
		Depends{ name: "ibase" }
		Depends{ name: "iprm" }
		Depends{ name: "iqt" }
		Depends{ name: "Qt.gui" }
		Depends{ name: "Qt.widgets" }
	}
}
