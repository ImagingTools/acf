import "../../../Config/Qbs/ApplicationProduct.qbs" as ApplicationProduct

ApplicationProduct{
	name: "Arxc"

	Depends{ name: "ipackage" }
	Depends{ name: "iqt" }
}
