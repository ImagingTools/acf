import "../../../Config/Qbs/ComponentProduct.qbs" as ComponentProduct

ComponentProduct{
	name: "BasePck"

	Depends{ name: "ilog" }
	Depends{ name: "ifile" }
	Depends{ name: "idoc" }
	Depends{ name: "ibase" }
	Depends{ name: "iprm" }
	Depends{ name: "i2d" }
	Depends{ name: "ipackage" }
}
