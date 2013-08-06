import qbs.base 1.0
import "../../../Config/Qbs/StaticProduct.qbs" as StaticProduct

StaticProduct{
	name: "imod"

	files: ["../*.h", "../*.cpp"]

	Export{
		Depends{ name: "iser" }
	}
}
