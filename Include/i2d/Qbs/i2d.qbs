import "../../../Config/Qbs/StaticProduct.qbs" as StaticProduct

StaticProduct{
	name: "i2d"

	Export {
		Depends{ name: "imath" }
		Depends{ name: "icomp" }
	}
}
