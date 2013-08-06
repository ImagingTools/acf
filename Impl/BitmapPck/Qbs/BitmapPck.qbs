import qbs.base 1.0
import "../../../Config/Qbs/ComponentProduct.qbs" as ComponentProduct

ComponentProduct{
	name: "BitmapPck"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "iimg" }
	Depends{ name: "ipackage" }
}
