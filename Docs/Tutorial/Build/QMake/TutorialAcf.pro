# Root of ACF project

include (../../../../Config/QMake/GeneralConfig.pri)

TEMPLATE = subdirs
CONFIG += ordered


#Static

SUBDIRS += itutacf
itutacf.file = ../../Include/itutacf/QMake/itutacf.pro

SUBDIRS += DocView
DocView.file = ../../Impl/DocView/QMake/DocView.pro

SUBDIRS += ModelObserver
ModelObserver.file = ../../Impl/ModelObserver/QMake/ModelObserver.pro


#Component

SUBDIRS += TutorialPck
TutorialPck.file = ../../Impl/TutorialPck/QMake/TutorialPck.pro
TutorialPck.depends = itutacf DocView ModelObserver


#Tool

SUBDIRS += DocViewSimu
DocViewSimu.file = ../../Impl/DocViewSimu/QMake/DocViewSimu.pro
DocViewSimu.depends = TutorialPck

SUBDIRS += ContainerGui
ContainerGui.file = ../../Impl/ContainerGui/QMake/ContainerGui.pro

