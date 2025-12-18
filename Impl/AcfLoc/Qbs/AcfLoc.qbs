import qbs.base 1.0
import AcfLibrary as AcfLibrary

AcfLibrary{
	Group{
		name: "Translations"
		files: ["../Translations/*.ts"]
	}

	files: ["../*.qrc"]
}
