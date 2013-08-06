import qbs.base 1.0
import "../../../Config/Qbs/StaticProduct.qbs" as StaticProduct

StaticProduct{
	name: "ifile"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "Qt.gui" }	// TODO: remove this dependency

	Export{
		Depends{ name: "ibase" }
		Depends{ name: "ilog" }
		Depends{ name: "Qt.gui" }	// TODO: remove this dependency
	}
}
