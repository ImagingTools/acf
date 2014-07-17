import qbs.base 1.0

Project{
	name: "TutorialAcf"

	property string projectSubDir: "Acf"
	property string projectRoot: path + "/../../.."
	property string projectSufixDir: "Tutorial"

	Project{
		name: "TutorialAcf-Static"

		references: [
			"../../Include/itutacf/Qbs/itutacf.qbs",
			"../../Impl/DocView/Qbs/DocView.qbs",
			"../../Impl/ModelObserver/Qbs/ModelObserver.qbs"
		]
	}

	Project{
		name: "TutorialAcf-Component"

		references: [
			"../../Impl/TutorialPck/Qbs/TutorialPck.qbs"
		]
	}

	Project{
		name: "TutorialAcf-Tool"

		references: [
			"../../Impl/DocViewSimu/Qbs/DocViewSimu.qbs",
			"../../Impl/ContainerGui/Qbs/ContainerGui.qbs"
		]
	}

	Product{
		name: "TutorialAcfConfig"
		type: ["xpc", "acf_share"]

		files: ["../../../../Config/PartituraOnly.xpc"]

		destinationDirectory: "Config"

		acf.xpcPackageDirs: path + "/../../Partitura"

		Depends{ name: "acf" }
		Depends{ name: "TutorialPck" }
		Depends{ name: "AcfCoreConfig" }
	}
}
