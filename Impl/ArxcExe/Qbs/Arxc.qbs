import qbs.base 1.0
import AcfApplication as AcfApplication

Project{
	StaticLibrary{
		name: "_Arxc"

		files: ["empty.cpp"]

		Depends{ name: "QtPck" }
		Depends{ name: "BasePck" }
		Depends{ name: "FilePck" }
		Depends{ name: "PackagePck" }
	}

	AcfApplication{
        name: "ArxcExe"
        targetName: "Arxc"

		consoleApplication: true

		files: ["../*.cpp"]

		cpp.includePaths: "../.."

		Depends{ name: "_Arxc" }

		Depends{ name: "ifile" }
		Depends{ name: "iqt" }
		Depends{ name: "ibase" }
		Depends{ name: "ipackage" }
	}
}
