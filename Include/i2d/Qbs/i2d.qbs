import qbs.base 1.0
import "../../../Config/Qbs/StaticProduct.qbs" as StaticProduct

StaticProduct{
	name: "i2d"

	files: ["../*.h", "../*.cpp"]

	Export{
		Depends{ name: "imath" }
		Depends{ name: "icomp" }
	}
}
