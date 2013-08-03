import "../../../Config/Qbs/StaticProduct.qbs" as StaticProduct

StaticProduct{
	name: "imath"

	Export {
		Depends{ name: "iser" }
	}
}
