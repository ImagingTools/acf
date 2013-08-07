import qbs.base 1.0
import "../../../Config/Qbs/StaticProduct.qbs" as StaticProduct

StaticProduct{
	name: "AcfLoc"

	Group{
		name: "translations"
		files: ["../*.ts"]
	}

	Group{
		name: "Resources"
		files: ["../*.ts", "../*.qrc"]
		fileTags: ["qm"]
	}

	Depends{ name: "Qt.core" }
}
