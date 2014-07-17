import qbs.base 1.0
import AcfApplication as AcfApplication

Project{
	StaticLibrary{
		name: "_DocViewSimuExe"

		files: ["empty.cpp"]

		Depends{ name: "BasePck" }
		Depends{ name: "QtPck" }
		Depends{ name: "QtGuiPck" }
		Depends{ name: "TutorialPck" }
	}

	AcfApplication{
		name: "DocViewSimuExe"
		targetName: "DocViewSimu"

		files: ["../*.cpp"]

		cpp.includePaths: "../../../../../Impl"

		Depends{ name: "_DocViewSimuExe" }

		Depends{ name: "iqtgui" }
		Depends{ name: "ibase" }
		Depends{ name: "iqtdoc" }
		Depends{ name: "DocView" }
		Depends{ name: "ModelObserver" }
		Depends{ name: "itutacf" }
	}
}
