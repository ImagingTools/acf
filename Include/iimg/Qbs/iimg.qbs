import "../../../Config/Qbs/StaticProduct.qbs" as StaticProduct

StaticProduct{
	name: "iimg"

	Export {
		Depends { name: "icomp" }
		Depends { name: "idoc" }
		Depends { name: "i2d" }
		Depends { name: "icmm" }
		Depends { name: "Qt.gui" }
	}
}
