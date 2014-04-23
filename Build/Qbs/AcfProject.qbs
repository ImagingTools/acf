import qbs.base 1.0

Project{
	name: "ACF-Core"
	id: acfCore

	Project{
		name: "ACF-Static"

		references: [
			"../../Impl/AcfLoc/Qbs/AcfLoc.qbs",
			"../../Include/istd/Qbs/istd.qbs",
			"../../Include/iser/Qbs/iser.qbs",
			"../../Include/ilog/Qbs/ilog.qbs",
			"../../Include/i2d/Qbs/i2d.qbs",
			"../../Include/iimg/Qbs/iimg.qbs",
			"../../Include/imath/Qbs/imath.qbs",
			"../../Include/icmm/Qbs/icmm.qbs",
			"../../Include/imod/Qbs/imod.qbs",
			"../../Include/icomp/Qbs/icomp.qbs",
			"../../Include/idoc/Qbs/idoc.qbs",
			"../../Include/iprm/Qbs/iprm.qbs",
			"../../Include/ibase/Qbs/ibase.qbs",
			"../../Include/iqt/Qbs/iqt.qbs",
			"../../Include/ifile/Qbs/ifile.qbs",
			"../../Include/iqtgui/Qbs/iwidgets.qbs",
			"../../Include/iqt2d/Qbs/iqt2d.qbs",
			"../../Include/iview/Qbs/iview.qbs",
			"../../Include/iqtdoc/Qbs/iqtdoc.qbs",
			"../../Include/iqtgui/Qbs/iqtgui.qbs",
			"../../Include/ifilegui/Qbs/ifilegui.qbs",
			"../../Include/iloggui/Qbs/iloggui.qbs",
			"../../Include/iqtprm/Qbs/iqtprm.qbs",
			"../../Include/ipackage/Qbs/ipackage.qbs"
		]
	}

	Project{
		name: "ACF-Component"

		references: [
			"../../Impl/BasePck/Qbs/BasePck.qbs",
			"../../Impl/BitmapPck/Qbs/BitmapPck.qbs",
			"../../Impl/QtPck/Qbs/QtPck.qbs",
			"../../Impl/FilePck/Qbs/FilePck.qbs",
			"../../Impl/QtGuiPck/Qbs/QtGuiPck.qbs",
			"../../Impl/QtViewPck/Qbs/QtViewPck.qbs",
			"../../Impl/PackagePck/Qbs/PackagePck.qbs",
			"../../Impl/FileGuiPck/Qbs/FileGuiPck.qbs"
		]
	}

	Project{
		name: "ACF-Tool"

		references: [
			"../../Impl/AcfExe/Qbs/Acf.qbs",
			"../../Impl/ArxcExe/Qbs/Arxc.qbs"
		]
	}

	Product{
		name: "AcfCoreConfig"
		type: ["xpc", "acf_share"]

		files: ["../../Config/PartituraOnly.xpc"]

		destinationDirectory: "Config"

		Depends{ name: "acf" }
		Depends{ name: "BasePck" }
		Depends{ name: "BitmapPck" }
		Depends{ name: "QtPck" }
        Depends{ name: "FilePck" }
        Depends{ name: "FileGuiPck" }
        Depends{ name: "QtGuiPck" }
		Depends{ name: "QtViewPck" }
		Depends{ name: "PackagePck" }
		Depends{ name: "FileGuiPck" }
	}
}
