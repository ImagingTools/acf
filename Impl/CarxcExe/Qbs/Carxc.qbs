import qbs.base 1.0
import AcfApplication as AcfApplication
import AcfLibrary as AcfLibrary

Project{
	StaticLibrary{
		name: "_Carxc"

		destinationDirectory: "GeneratedFiles/Carxc"

		files: ["../*.arx", "../*.qrc"]

		Depends{ name: "acf" }

		Depends{ name: "ArxcExe" }

		Depends{ name: "AcfSlnConfig" }

		Depends{ name: "CarxcPck" }
		Depends{ name: "QtGuiPck" }
		Depends{ name: "QtPck" }
		Depends{ name: "FilePck" }
        Depends{ name: "FileGuiPck" }
        Depends{ name: "BasePck" }
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

		consoleApplication: false

		files: ["../*.cpp"]

		Depends{ name: "_Carxc" }

		Depends{ name: "AcfSlnConfig" }
		Depends{ name: "AcfSlnLoc" }
		Depends{ name: "AcfLoc" }

		Depends{ name: "icmpstr" }
		Depends{ name: "iqtgui" }
		Depends{ name: "ifile" }
		Depends{ name: "ipackage" }

		Depends{ name: "Qt.widgets" }
		Depends{ name: "Qt.gui" }

		// Resource file on MAC
		Group{
            name: "MAC-Resources"
            condition: qbs.targetOS.contains("osx")

            cpp.infoPlistFile: "../Mac/Info.plist"
            cpp.infoPlistFormat: "xml1"
        }

		// Resource file on Windows
		Depends{ name: "AcfExe" }
		acf.trRegFile: "../../../Partitura/AcfInfoCopyApp.arx"
		acf.trOutputType: "rc"
		cpp.includePaths: path

		Group{
			name: "Windows-Resources"
			condition: qbs.targetOS.contains("windows")

			files: ["../VC/*.rc.xtracf"]
			fileTags: ["xtracf"]
		}
	}
}
