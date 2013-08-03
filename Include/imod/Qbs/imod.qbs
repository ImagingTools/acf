import "../../../Config/Qbs/StaticProduct.qbs" as StaticProduct

StaticProduct{
	name: "imod"

	Export {
		Depends { name: "iser" }
	}
}
