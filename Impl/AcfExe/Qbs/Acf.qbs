import qbs.base 1.0
import AcfConsoleApplication as AcfConsoleApplication

AcfConsoleApplication{
	name: "AcfExe"
	targetName: "Acf"

	files: ["../*.cpp"]

	Depends{ name: "ipackage" }
	Depends{ name: "icomp" }
	Depends{ name: "Qt.widgets" }
	Depends{ name: "Qt.gui" }
}
