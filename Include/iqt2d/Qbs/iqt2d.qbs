import qbs.base 1.0
import "../../../Config/Qbs/StaticProduct.qbs" as StaticProduct

StaticProduct{
	name: "iqt2d"

	files: ["../*.h", "../*.cpp", "../*.ui", "../*.qrc"]

	Depends{ name: "iview" }

	Export{
		Depends{ name: "i2d" }
		Depends{ name: "iview" }
	}
}
