import qbs.base 1.0
import "../../../Config/Qbs/ComponentProduct.qbs" as ComponentProduct

ComponentProduct{
	name: "BasePck"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "ilog" }
	Depends{ name: "ifile" }
	Depends{ name: "idoc" }
	Depends{ name: "ibase" }
	Depends{ name: "iprm" }
	Depends{ name: "i2d" }
	Depends{ name: "ipackage" }
}
