import "../../../Config/Qbs/StaticProduct.qbs" as StaticProduct

StaticProduct{
	name: "ibase"

	Export {
		Depends { name: "ilog" }
		Depends { name: "icomp" }
	}
}
