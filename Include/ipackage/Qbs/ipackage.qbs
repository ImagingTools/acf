import qbs.base 1.0
import "../../../Config/Qbs/StaticProduct.qbs" as StaticProduct

StaticProduct{
	name: "ipackage"

	files: ["../*.h", "../*.cpp"]

    Depends{ name: "ifile" }

    Export{
		Depends{ name: "ilog" }
		Depends{ name: "iprm" }
	}
}
