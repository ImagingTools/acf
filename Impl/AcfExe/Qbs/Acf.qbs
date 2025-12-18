import qbs.base 1.0
import AcfConsoleApplication as AcfConsoleApplication

AcfConsoleApplication{
	condition: acf.acfToolsBin == null

	name: "AcfExe"
	targetName: "Acf"

	files: ["../*.cpp"]

	Depends{ name: "ipackage" }
	Depends{ name: "icomp" }
	Depends{ name: "Qt.widgets" }
	Depends{ name: "Qt.gui" }
}
