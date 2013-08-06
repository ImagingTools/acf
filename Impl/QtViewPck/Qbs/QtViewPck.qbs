import qbs.base 1.0
import "../../../Config/Qbs/ComponentProduct.qbs" as ComponentProduct

ComponentProduct{
	name: "QtViewPck"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "iqtgui" }
	Depends{ name: "iview" }
}
