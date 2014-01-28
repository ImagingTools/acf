import qbs.base 1.0
import AcfApplication as AcfApplication

AcfApplication{
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
