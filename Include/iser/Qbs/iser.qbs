import qbs.base 1.0
import "../../../Config/Qbs/StaticProduct.qbs" as StaticProduct

StaticProduct{
	name: "iser"

	files: ["../*.h", "../*.cpp"]

	Export{
		Depends{ name: "istd" }
	}
}
