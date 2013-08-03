import "../../../Config/Qbs/ComponentProduct.qbs" as ComponentProduct

ComponentProduct{
	name: "BitmapPck"

	Depends{ name: "iimg" }
	Depends{ name: "ipackage" }
}
