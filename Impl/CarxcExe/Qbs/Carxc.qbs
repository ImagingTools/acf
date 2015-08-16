import qbs.base 1.0
import AcfApplication as AcfApplication
import AcfLibrary as AcfLibrary

Project{
	StaticLibrary{
		name: "_Carxc"

		destinationDirectory: "GeneratedFiles/Carxc"

		files: ["../*.arx"]

		Depends{ name: "acf" }

		Depends{ name: "ArxcExe" }

		Depends{ name: "AcfCoreConfig" }

		Depends{ name: "BasePck" }
		Depends{ name: "QtPck" }
		Depends{ name: "PackagePck" }

		acf.generatedOutputDir: "GeneratedFiles/Carxc"

		Export{
			Depends{ name: "cpp" }
			cpp.includePaths: [product.buildDirectory]
		}
	}

	AcfApplication{
		name: "CarxcExe"
		targetName: "Carxc"

		consoleApplication: true

		files: ["../*.cpp"]

		Depends{ name: "_Carxc" }

		Depends{ name: "iattr" }
		Depends{ name: "ibase" }
		Depends{ name: "icomp" }
		Depends{ name: "ifile" }
		Depends{ name: "ilog" }
		Depends{ name: "imod" }
		Depends{ name: "ipackage" }
		Depends{ name: "iqt" }
		Depends{ name: "iser" }
		Depends{ name: "istd" }
	}
}
