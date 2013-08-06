import qbs.base 1.0

Project{
	name: "Acf"

	property path acfDir: "../.."

	SubProject{
		filePath: "Static.qbs"
	}
	SubProject{
		filePath: "Component.qbs"
	}
	SubProject{
		filePath: "Tool.qbs"
	}
}
