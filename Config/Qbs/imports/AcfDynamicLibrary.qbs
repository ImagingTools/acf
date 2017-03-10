import qbs.base 1.0

Product{
	type: [(qbs.hostOS.contains('windows') && qbs.targetOS.contains('windows') && qbs.toolchain.contains('msvc'))? "acf_dynamiclibrary": "dynamiclibrary", "acf_share"]

	Depends{ name: "cpp" }
	Depends{ name: "acf" }
	Depends{ name: "acf.extensions" }
}
