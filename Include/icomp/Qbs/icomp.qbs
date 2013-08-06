import qbs.base 1.0
import "../../../Config/Qbs/StaticProduct.qbs" as StaticProduct

StaticProduct{
	name: "icomp"

	files: ["../*.h", "../*.cpp"]

	Export{
		Depends{ name: "imod" }
	}
}
