import "../../../Config/Qbs/StaticProduct.qbs" as StaticProduct

StaticProduct{
	name: "iloggui"

	Export {
		Depends { name: "ilog" }
		Depends { name: "iqtgui" }
	}
}
