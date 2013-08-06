import qbs.base 1.0
import "../../../Config/Qbs/ComponentProduct.qbs" as ComponentProduct

ComponentProduct{
	name: "QtPck"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "iqt" }
	Depends{ name: "iimg" }
	Depends{ name: "ifile" }
	Depends{ name: "iqtdoc" }
}
