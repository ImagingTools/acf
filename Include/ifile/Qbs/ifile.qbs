import "../../../Config/Qbs/StaticProduct.qbs" as StaticProduct

StaticProduct{
	name: "ifile"

	Export {
		Depends { name: "ibase" }
		Depends { name: "ilog" }
		Depends { name: "Qt.gui" }
	}
}
