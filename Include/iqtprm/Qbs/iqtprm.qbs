import qbs.base 1.0
import "../../../Config/Qbs/StaticProduct.qbs" as StaticProduct

StaticProduct{
	name: "iqtprm"

	files: ["../*.h", "../*.cpp", "../*.ui"]

	Depends{ name: "iprm" }
	Depends{ name: "iqtgui" }
	Depends{ name: "Qt.gui" }
	Depends{ name: "Qt.widgets" }

	Export{
		Depends{ name: "iprm" }
		Depends{ name: "iqtgui" }
		Depends{ name: "Qt.gui" }
		Depends{ name: "Qt.widgets" }
	}
}
