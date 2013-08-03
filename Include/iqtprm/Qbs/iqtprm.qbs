import "../../../Config/Qbs/StaticProduct.qbs" as StaticProduct

StaticProduct{
	name: "iqtprm"

	Export {
		Depends { name: "iprm" }
		Depends { name: "iqtgui" }
	}
}
