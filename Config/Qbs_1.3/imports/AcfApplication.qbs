import qbs.base 1.0
// Standard settings for an ACF application

CppApplication{
	type: [
		qbs.targetOS.contains("darwin")? "applicationbundle": "application",
		"acf_share"
	]

	consoleApplication: true

	Depends{ name: "acf" }
	Depends{ name: "Qt.core" }

	Properties{
		condition: qbs.toolchain.contains("gcc") || qbs.toolchain.contains("clang")
		cpp.cxxFlags: ['-Wno-format-security', '-Wno-unknown-pragmas']
	}

	cpp.defines: ['I_QBS']

	Group{
		condition: acf.installProject === undefined || acf.installProject == acf.projectName
		fileTagsFilter: ["application"]
		qbs.install: true
		qbs.installDir: acf.targetBinSubdir
	}
}
