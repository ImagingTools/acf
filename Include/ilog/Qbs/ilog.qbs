import "../../../Config/Qbs/StaticProduct.qbs" as StaticProduct

StaticProduct{
	name: "ilog"

	Export {
		Depends { name: "imod" }
		Depends { name: "iser" }
		Depends { name: "icomp" }
	}
}
