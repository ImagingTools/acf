import qbs.base 1.0
import "../../../Config/Qbs/ApplicationProduct.qbs" as ApplicationProduct

ApplicationProduct{
	name: "AcfExe"
	targetName: "Acf"

	consoleApplication: true

	type: ["application", "acfTool"]

	files: ["../*.cpp"]

	Depends{ name: "ipackage" }
	Depends{ name: "icomp" }
	Depends{ name: "Qt.widgets" }
	Depends{ name: "Qt.gui" }
}
