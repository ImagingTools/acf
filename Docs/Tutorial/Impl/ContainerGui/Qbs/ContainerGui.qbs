import qbs.base 1.0
import AcfApplication as AcfApplication

AcfApplication{
	name: "ContainerGuiExe"
	targetName: "ContainerGui"

	files: ["../*.cpp"]

	Depends{ name: "iwidgets" }
	Depends{ name: "iqtgui" }
}
