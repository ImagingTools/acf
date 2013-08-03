import "../../../Config/Qbs/ComponentProduct.qbs" as ComponentProduct

ComponentProduct{
	name: "PackagePck"

	Depends{
		name: "ilog"
	}
	Depends{
		name: "ipackage"
	}
}
