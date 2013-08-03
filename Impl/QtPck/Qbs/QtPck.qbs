import "../../../Config/Qbs/ComponentProduct.qbs" as ComponentProduct

ComponentProduct{
	name: "QtPck"

	Depends{
		name: "ifile"
	}
	Depends{
		name: "iqtdoc"
	}
}
