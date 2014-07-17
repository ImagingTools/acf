import qbs.base 1.0
import AcfPackage as AcfPackage

AcfPackage{
	name: "TutorialPck"

	files: ["../*.h", "../*.cpp"]

	Depends{ name: "icomp" }
	Depends{ name: "DocView" }
	Depends{ name: "ModelObserver" }
	Depends{ name: "itutacf" }

	Export{
		Depends{ name: "icomp" }
		Depends{ name: "DocView" }
		Depends{ name: "ModelObserver" }
		Depends{ name: "itutacf" }
	}
}
