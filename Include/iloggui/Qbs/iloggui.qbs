import qbs.base 1.0
import "../../../Config/Qbs/StaticProduct.qbs" as StaticProduct

StaticProduct{
	name: "iloggui"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "iqtgui" }

	Export{
		Depends{ name: "ilog" }
		Depends{ name: "iqtgui" }
	}
}
