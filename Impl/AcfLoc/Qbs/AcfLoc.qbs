import qbs.base 1.0
import AcfStaticLibrary as AcfStaticLibrary

AcfStaticLibrary{
	name: "AcfLoc"

	Group{
		name: "Translations"
		files: ["../*.ts"]
	}

	Group{
		name: "Resources"
		files: ["../*.qrc"]
		fileTags: ["qrc_generated"]
	}

	Qt.core.qmFilesDir: 'GeneratedFiles/' + product.name
}
