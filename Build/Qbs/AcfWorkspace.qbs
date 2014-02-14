import qbs.base 1.0

Project{
    qbsSearchPaths: ["../../Config/Qbs"]

	SubProject{
		filePath: "./AcfProject.qbs"
	}
}
