import qbs.base 1.0
import "../../../Config/Qbs/StaticProduct.qbs" as StaticProduct

StaticProduct{
	name: "iqt"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "Qt.xml" }

	Export{
		Depends{ name: "icomp" }
		Depends{ name: "Qt.xml" }
	}
}
