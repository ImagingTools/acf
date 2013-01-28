TEMPLATE = subdirs
CONFIG += ordered

SUBDIRS += Acf
SUBDIRS += Arxc
SUBDIRS += CompositorMini
SUBDIRS += Compositor
SUBDIRS += XpcEditor

Acf.file = ../../Impl/AcfExe/QMake/Acf.pro
Arxc.file = ../../Impl/ArxcExe/QMake/Arxc.pro
Arxc.depends = Acf
CompositorMini.file = ../../Impl/CompositorMiniExe/QMake/CompositorMini.pro
CompositorMini.depends = Acf
Compositor.file = ../../Impl/CompositorExe/QMake/Compositor.pro
Compositor.depends = Arxc
XpcEditor.file = ../../Impl/XpcEditorExe/QMake/XpcEditor.pro
XpcEditor.depends = Arxc
