import "../../../Config/Qbs/StaticProduct.qbs" as StaticProduct

StaticProduct{
	name: "iqt"

	Export {
		Depends { name: "icomp" }
		Depends { name: "iimg" }
		Depends { name: "Qt.xml" }
	}
}
