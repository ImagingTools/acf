import qbs.base 1.0
import AcfStaticLibrary as AcfStaticLibrary

AcfStaticLibrary{
	name: "iqtdoc"

	files: ["../*.h", "../*.cpp", "../*.ui"]

	Depends{ name: "ifilegui" }
	Depends{ name: "idoc" }
	Depends{ name: "Qt.widgets" }
	Depends{ name: "Qt.gui" }

	Export{
		Depends{ name: "ifilegui" }
		Depends{ name: "idoc" }
		Depends{ name: "Qt.gui" }
		Depends{ name: "Qt.widgets" }
	}
}
