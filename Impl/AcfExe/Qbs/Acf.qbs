import qbs.base 1.0
import "../../../Config/Qbs/ApplicationProduct.qbs" as ApplicationProduct

ApplicationProduct{
	name: "Acf"

	type: ["application", "acfTool"]
	consoleApplication: true

	files: ["../*.cpp"]

	Depends{ name: "ipackage" }
	Depends{ name: "icomp" }
	Depends{ name: "Qt.widgets" }
	Depends{ name: "Qt.gui" }
}
