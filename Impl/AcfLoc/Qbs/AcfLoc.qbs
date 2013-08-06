import qbs.base 1.0
import "../../../Config/Qbs/StaticProduct.qbs" as StaticProduct

StaticProduct{
	name: "AcfLoc"

	files: ["../*.ts", "../*.qrc"]
}
