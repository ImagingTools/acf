import qbs.base 1.0
import "../../../Config/Qbs/StaticProduct.qbs" as StaticProduct

StaticProduct{
	name: "ibase"

	files: ["../*.h", "../*.cpp"]

	Export{
		Depends{ name: "ilog" }
		Depends{ name: "icomp" }
	}
}
