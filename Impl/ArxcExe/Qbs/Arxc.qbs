import qbs.base 1.0
import AcfConsoleApplication as AcfConsoleApplication

Project{
	StaticLibrary{
		condition: acf.acfToolsBin == null

		name: "_Arxc"

		files: ["empty.cpp"]

		Depends{ name: "acf" }
		Depends{ name: "QtPck" }
		Depends{ name: "BasePck" }
		Depends{ name: "FilePck" }
		Depends{ name: "PackagePck" }
	}

	AcfConsoleApplication{
		condition: acf.acfToolsBin == null

		name: "ArxcExe"
		targetName: "Arxc"

		files: ["../*.cpp"]

		cpp.includePaths: "../.."

		Depends{ name: "_Arxc" }

		Depends{ name: "ifile" }
		Depends{ name: "iqt" }
		Depends{ name: "ibase" }
		Depends{ name: "ipackage" }
	}
}
