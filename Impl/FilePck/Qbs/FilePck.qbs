import "../../../Config/Qbs/ComponentProduct.qbs" as ComponentProduct

ComponentProduct{
	name: "FilePck"

	Depends{
		name: "ifile"
	}
	Depends{
		name: "ilog"
	}
}
