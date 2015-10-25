# Root of ACF project


include (../../Config/QMake/Solution.pri)

#Static

SUBDIRS += istd
istd.file = ../../Include/istd/QMake/istd.pro

SUBDIRS += iser
iser.file = ../../Include/iser/QMake/iser.pro

SUBDIRS += ilog
ilog.file = ../../Include/ilog/QMake/ilog.pro

SUBDIRS += i2d
i2d.file = ../../Include/i2d/QMake/i2d.pro

SUBDIRS += i3d
i3d.file = ../../Include/i3d/QMake/i3d.pro

SUBDIRS += iattr
iattr.file = ../../Include/iattr/QMake/iattr.pro

SUBDIRS += iimg
iimg.file = ../../Include/iimg/QMake/iimg.pro

SUBDIRS += imath
imath.file = ../../Include/imath/QMake/imath.pro

SUBDIRS += icmm
icmm.file = ../../Include/icmm/QMake/icmm.pro

SUBDIRS += imod
imod.file = ../../Include/imod/QMake/imod.pro

SUBDIRS += icomp
icomp.file = ../../Include/icomp/QMake/icomp.pro

SUBDIRS += idoc
idoc.file = ../../Include/idoc/QMake/idoc.pro

SUBDIRS += iprm
iprm.file = ../../Include/iprm/QMake/iprm.pro

SUBDIRS += ibase
ibase.file = ../../Include/ibase/QMake/ibase.pro

SUBDIRS += iqt
iqt.file = ../../Include/iqt/QMake/iqt.pro

SUBDIRS += ifile
ifile.file = ../../Include/ifile/QMake/ifile.pro

SUBDIRS += iqt2d
iqt2d.file = ../../Include/iqt2d/QMake/iqt2d.pro

SUBDIRS += iview
iview.file = ../../Include/iview/QMake/iview.pro

SUBDIRS += iqtdoc
iqtdoc.file = ../../Include/iqtdoc/QMake/iqtdoc.pro

SUBDIRS += iwidgets
iwidgets.file = ../../Include/iwidgets/QMake/iwidgets.pro

SUBDIRS += iqtgui
iqtgui.file = ../../Include/iqtgui/QMake/iqtgui.pro

SUBDIRS += ifilegui
ifilegui.file = ../../Include/ifilegui/QMake/ifilegui.pro

SUBDIRS += iloggui
iloggui.file = ../../Include/iloggui/QMake/iloggui.pro

SUBDIRS += iqtprm
iqtprm.file = ../../Include/iqtprm/QMake/iqtprm.pro

SUBDIRS += ipackage
ipackage.file = ../../Include/ipackage/QMake/ipackage.pro


# Translation

SUBDIRS += AcfLoc
AcfLoc.file = ../../Impl/AcfLoc/QMake/AcfLoc.pro


# Component packages (build only for desktop platforms)
!macx-ios*{
	SUBDIRS += BasePck
	BasePck.file = ../../Impl/BasePck/QMake/BasePck.pro

	SUBDIRS += BitmapPck
	BitmapPck.file = ../../Impl/BitmapPck/QMake/BitmapPck.pro

	SUBDIRS += QtPck
	QtPck.file = ../../Impl/QtPck/QMake/QtPck.pro

	SUBDIRS += FilePck
	FilePck.file = ../../Impl/FilePck/QMake/FilePck.pro

	SUBDIRS += QtGuiPck
	QtGuiPck.file = ../../Impl/QtGuiPck/QMake/QtGuiPck.pro

	SUBDIRS += QtViewPck
	QtViewPck.file = ../../Impl/QtViewPck/QMake/QtViewPck.pro

	SUBDIRS += PackagePck
	PackagePck.file = ../../Impl/PackagePck/QMake/PackagePck.pro

	SUBDIRS += FileGuiPck
	FileGuiPck.file = ../../Impl/FileGuiPck/QMake/FileGuiPck.pro
}

# Tools (build only for desktop platforms)
!macx-ios*{
	SUBDIRS += Acf
	Acf.file = ../../Impl/AcfExe/QMake/Acf.pro

	SUBDIRS += Arxc
	Arxc.file = ../../Impl/ArxcExe/QMake/Arxc.pro
	Arxc.depends = Acf
}
