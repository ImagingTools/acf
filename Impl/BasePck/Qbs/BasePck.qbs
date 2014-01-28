import qbs.base 1.0
import AcfPackage as AcfPackage

AcfPackage{
	name: "BasePck"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "ilog" }
	Depends{ name: "ifile" }
	Depends{ name: "idoc" }
	Depends{ name: "ibase" }
	Depends{ name: "iprm" }
	Depends{ name: "i2d" }

	Export{
		Depends{ name: "ilog" }
		Depends{ name: "ifile" }
		Depends{ name: "idoc" }
		Depends{ name: "ibase" }
		Depends{ name: "iprm" }
		Depends{ name: "i2d" }
	}
}
