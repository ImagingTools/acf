import qbs.base 1.0

Project{
	name: "ACF-Component"

	references: [
		"../../Impl/BasePck/Qbs/BasePck.qbs",
		"../../Impl/BitmapPck/Qbs/BitmapPck.qbs",
		"../../Impl/QtPck/Qbs/QtPck.qbs",
		"../../Impl/FilePck/Qbs/FilePck.qbs",
		"../../Impl/QtGuiPck/Qbs/QtGuiPck.qbs",
		"../../Impl/QtViewPck/Qbs/QtViewPck.qbs",
		"../../Impl/PackagePck/Qbs/PackagePck.qbs"
	]
}

