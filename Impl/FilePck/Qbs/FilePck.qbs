import qbs.base 1.0
import "../../../Config/Qbs/ComponentProduct.qbs" as ComponentProduct

ComponentProduct{
	name: "FilePck"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "ifile" }
	Depends{ name: "ilog" }
}
