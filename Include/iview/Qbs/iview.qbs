import "../../../Config/Qbs/StaticProduct.qbs" as StaticProduct

StaticProduct{
	name: "iview"

	Export {
		Depends { name: "iqt2d" }
		Depends { name: "iqtgui" }
	}
}
