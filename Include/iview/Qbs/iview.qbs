import qbs.base 1.0
import "../../../Config/Qbs/StaticProduct.qbs" as StaticProduct

StaticProduct{
	name: "iview"

	files: ["../*.h", "../*.cpp", "../*.ui"]

	Depends{ name: "iqt2d" }
	Depends{ name: "iqtgui" }
	Depends{ name: "Qt.widgets" }
	Depends{ name: "Qt.gui" }

	Export{
		Depends{ name: "iqt2d" }
		Depends{ name: "iqtgui" }
		Depends{ name: "Qt.widgets" }
		Depends{ name: "Qt.gui" }
	}
}
