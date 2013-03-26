# Root of ACF project

include (../../Config/QMake/GeneralConfig.pri)

TEMPLATE = subdirs
CONFIG += ordered


#Static

SUBDIRS += istd
istd.file = ../../Include/istd/QMake/istd.pro

SUBDIRS += iser
iser.file = ../../Include/iser/QMake/iser.pro

SUBDIRS += i2d
i2d.file = ../../Include/i2d/QMake/i2d.pro

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

SUBDIRS += iqtgui
iqtgui.file = ../../Include/iqtgui/QMake/iqtgui.pro

SUBDIRS += iqtprm
iqtprm.file = ../../Include/iqtprm/QMake/iqtprm.pro

SUBDIRS += icmpstr
icmpstr.file = ../../Include/icmpstr/QMake/icmpstr.pro


# Translation

SUBDIRS += AcfLoc
AcfLoc.file = ../../Impl/AcfLoc/QMake/AcfLoc.pro


#Component

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

SUBDIRS += CompositorPck
CompositorPck.file = ../../Impl/CompositorPck/QMake/CompositorPck.pro


#Tool

SUBDIRS += Acf
Acf.file = ../../Impl/AcfExe/QMake/Acf.pro

SUBDIRS += Arxc
Arxc.file = ../../Impl/ArxcExe/QMake/Arxc.pro
Arxc.depends = Acf

SUBDIRS += CompositorMini
CompositorMini.file = ../../Impl/CompositorMiniExe/QMake/CompositorMini.pro

SUBDIRS += Compositor
Compositor.file = ../../Impl/CompositorExe/QMake/Compositor.pro
Compositor.depends = Arxc

SUBDIRS += XpcEditor
XpcEditor.file = ../../Impl/XpcEditorExe/QMake/XpcEditor.pro
XpcEditor.depends = Arxc

