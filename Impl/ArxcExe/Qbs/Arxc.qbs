import qbs.base 1.0
import "../../../Config/Qbs/ApplicationProduct.qbs" as ApplicationProduct

ApplicationProduct{
	name: "Arxc"

	files: ["../*.cpp"]

	Depends{ name: "iqt" }
	Depends{ name: "ipackage" }
	Depends{ name: "Qt.widgets" }
	Depends{ name: "Qt.gui" }
}
