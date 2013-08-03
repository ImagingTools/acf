import "../../../Config/Qbs/StaticProduct.qbs" as StaticProduct

StaticProduct{
	name: "icomp"

	Export {
		Depends { name: "imod" }
	}
}
