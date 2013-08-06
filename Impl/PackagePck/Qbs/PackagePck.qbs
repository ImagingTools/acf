import qbs.base 1.0
import "../../../Config/Qbs/ComponentProduct.qbs" as ComponentProduct

ComponentProduct{
	name: "PackagePck"

	files: ["../*.h", "../*.cpp"]

    Depends{ name: "ipackage" }
    Depends{ name: "ilog" }
}
