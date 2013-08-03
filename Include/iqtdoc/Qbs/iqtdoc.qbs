import "../../../Config/Qbs/StaticProduct.qbs" as StaticProduct

StaticProduct{
	name: "iqtdoc"

	Export {
		Depends { name: "idoc" }
		Depends { name: "iqtgui" }
	}
}
