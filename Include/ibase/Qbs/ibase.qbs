import qbs.base 1.0
import AcfLibrary as AcfLibrary

AcfLibrary{
	files: ["../*.h", "../*.cpp"]

    Depends{ name: "ilog" }
    Depends{ name: "icomp" }
    Depends{ name: "iprm" }

	Export{
		Depends{ name: "ilog" }
		Depends{ name: "icomp" }
        Depends{ name: "iprm" }
    }
}
