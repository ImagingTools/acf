import qbs.base 1.0
import AcfStaticLibrary as AcfStaticLibrary

AcfStaticLibrary{
	name: "ibase"

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
