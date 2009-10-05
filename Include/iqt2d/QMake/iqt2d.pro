TARGET = iqt2d
TEMPLATE = lib
CONFIG += staticlib

CONFIG(debug, debug|release) {
	DESTDIR = ..\..\..\Lib\DebugQMake
}
CONFIG(release, debug|release) {
	DESTDIR = ..\..\..\Lib\ReleaseQMake
}

UI_DIR = ../Generated
MOC_DIR = ../Generated
RCC_DIR = ../Generated

QT += 


INCLUDEPATH += ../../ "$(QTDIR)/include" "$(QTDIR)/include/QtCore" "$(QTDIR)/include/QtGui" "$(QTDIR)/include/QtXml" 

HEADERS += ../CAnnulusParamsGuiComp.h
HEADERS += ../CAnnulusSegmentParamsGuiComp.h
HEADERS += ../CAnnulusSegmentShape.h
HEADERS += ../CAnnulusShape.h
HEADERS += ../CCircleParamsGuiComp.h
HEADERS += ../CCircleShape.h
HEADERS += ../CGripShape.h
HEADERS += ../CImageShape.h
HEADERS += ../CImageViewComp.h
HEADERS += ../CLine2dParamsGuiComp.h
HEADERS += ../CLine2dShape.h
HEADERS += ../CPosition2dShape.h
HEADERS += ../CQuadrangleParamsGuiComp.h
HEADERS += ../CQuadrangleShape.h
HEADERS += ../CRectangleParamsGuiComp.h
HEADERS += ../CRectangleShape.h
HEADERS += ../CSceneConnectorGuiComp.h
HEADERS += ../CSceneControllerGuiComp.h
HEADERS += ../CSceneProviderGuiComp.h
HEADERS += ../iqt2d.h
HEADERS += ../ISceneExtender.h
HEADERS += ../ISceneProvider.h
HEADERS += ../TObjectShapeBase.h
HEADERS += ../TSceneExtenderCompBase.h
HEADERS += ../TShapeBase.h
HEADERS += ../generated\ui_CAnnulusParamsGuiComp.h
HEADERS += ../generated\ui_CAnnulusSegmentParamsGuiComp.h
HEADERS += ../generated\ui_CCircleParamsGuiComp.h
HEADERS += ../generated\ui_CLine2dParamsGuiComp.h
HEADERS += ../generated\ui_CQuadrangleParamsGuiComp.h
HEADERS += ../generated\ui_CRectangleParamsGuiComp.h
HEADERS += ../generated\ui_CSceneConnectorGuiComp.h
HEADERS += ../generated\ui_CSceneControllerGuiComp.h
HEADERS += ../generated\ui_CSceneProviderGuiComp.h
SOURCES += ../CAnnulusParamsGuiComp.cpp
SOURCES += ../CAnnulusSegmentParamsGuiComp.cpp
SOURCES += ../CAnnulusSegmentShape.cpp
SOURCES += ../CAnnulusShape.cpp
SOURCES += ../CCircleParamsGuiComp.cpp
SOURCES += ../CCircleShape.cpp
SOURCES += ../CGripShape.cpp
SOURCES += ../CImageShape.cpp
SOURCES += ../CImageViewComp.cpp
SOURCES += ../CLine2dParamsGuiComp.cpp
SOURCES += ../CLine2dShape.cpp
SOURCES += ../CPosition2dShape.cpp
SOURCES += ../CQuadrangleParamsGuiComp.cpp
SOURCES += ../CQuadrangleShape.cpp
SOURCES += ../CRectangleParamsGuiComp.cpp
SOURCES += ../CRectangleShape.cpp
SOURCES += ../CSceneConnectorGuiComp.cpp
SOURCES += ../CSceneControllerGuiComp.cpp
SOURCES += ../CSceneProviderGuiComp.cpp
SOURCES += ../generated\moc_CAnnulusParamsGuiComp.cpp
SOURCES += ../generated\moc_CAnnulusSegmentParamsGuiComp.cpp
SOURCES += ../generated\moc_CAnnulusSegmentShape.cpp
SOURCES += ../generated\moc_CAnnulusShape.cpp
SOURCES += ../generated\moc_CCircleParamsGuiComp.cpp
SOURCES += ../generated\moc_CCircleShape.cpp
SOURCES += ../generated\moc_CGripShape.cpp
SOURCES += ../generated\moc_CLine2dParamsGuiComp.cpp
SOURCES += ../generated\moc_CLine2dShape.cpp
SOURCES += ../generated\moc_CQuadrangleParamsGuiComp.cpp
SOURCES += ../generated\moc_CQuadrangleShape.cpp
SOURCES += ../generated\moc_CRectangleParamsGuiComp.cpp
SOURCES += ../generated\moc_CRectangleShape.cpp
SOURCES += ../generated\moc_CSceneControllerGuiComp.cpp
SOURCES += ../generated\moc_CSceneProviderGuiComp.cpp
FORMS += ../CAnnulusParamsGuiComp.ui
FORMS += ../CAnnulusSegmentParamsGuiComp.ui
FORMS += ../CCircleParamsGuiComp.ui
FORMS += ../CLine2dParamsGuiComp.ui
FORMS += ../CQuadrangleParamsGuiComp.ui
FORMS += ../CRectangleParamsGuiComp.ui
FORMS += ../CSceneConnectorGuiComp.ui
FORMS += ../CSceneControllerGuiComp.ui
FORMS += ../CSceneProviderGuiComp.ui
