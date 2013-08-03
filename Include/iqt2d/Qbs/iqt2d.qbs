import "../../../Config/Qbs/StaticProduct.qbs" as StaticProduct

StaticProduct{
	name: "iqt2d"

	Export {
		Depends { name: "i2d" }
		Depends { name: "iview" }
	}
}
