import "../../../Config/Qbs/StaticProduct.qbs" as StaticProduct

StaticProduct{
	name: "ipackage"

	Export {
		Depends { name: "ilog" }
		Depends { name: "ifile" }
		Depends { name: "iprm" }
	}
}
