import qbs.base 1.0
import "../../../Config/Qbs/ApplicationProduct.qbs" as ApplicationProduct

Project{
	StaticLibrary{
		name: "_Arxc"

		files: ["empty.cpp"]

		Depends{ name: "QtPck" }
		Depends{ name: "BasePck" }
		Depends{ name: "FilePck" }
		Depends{ name: "PackagePck" }
	}

	ApplicationProduct{
		name: "Arxc"

		files: ["../*.cpp"]

		cpp.includePaths: "../.."

		Depends{ name: "_Arxc" }

		Depends{ name: "ifile" }
		Depends{ name: "iqt" }
		Depends{ name: "ibase" }
		Depends{ name: "ipackage" }

		consoleApplication: true
	}
}
