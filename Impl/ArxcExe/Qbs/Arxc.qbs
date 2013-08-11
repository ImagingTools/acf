import qbs.base 1.0
import "../../../Config/Qbs/ApplicationProduct.qbs" as ApplicationProduct

ApplicationProduct{
	name: "Arxc"

	type: ["application", "acfTool"]
	consoleApplication: true

	files: ["../*.cpp"]

	Depends{ name: "QtPck" }
	Depends{ name: "BasePck" }
	Depends{ name: "FilePck" }
	Depends{ name: "PackagePck" }
}
