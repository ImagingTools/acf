import "../../../Config/Qbs/ComponentProduct.qbs" as ComponentProduct

ComponentProduct{
	name: "QtGuiPck"

	Depends{
		name: "icomp"
	}
	Depends{
		name: "iqtgui"
	}
}
