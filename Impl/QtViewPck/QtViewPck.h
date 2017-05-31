#ifndef QtViewPck_included
#define QtViewPck_included


// ACF includes
#include <icomp/TModelCompWrap.h>
#include <icomp/TMakeComponentWrap.h>
#include <ibase/TModelObserverCompWrap.h>
#include <iview/CShapeStatusInfoComp.h>
#include <iview/CShapeInfoGuiComp.h>
#include <iview/CSimpleShapeFactoryComp.h>
#include <iview/CColorSchemaComp.h>
#include <iqt2d/TViewExtenderDelegatorCompWrap.h>
#include <iqt2d/TMultiViewExtenderDelegatorCompWrap.h>
#include <iqt2d/CSceneConnectorGuiComp.h>
#include <iqt2d/CImageViewComp.h>
#include <iqt2d/CPosition2dParamsGuiComp.h>
#include <iqt2d/CLine2dParamsGuiComp.h>
#include <iqt2d/CRectangleParamsGuiComp.h>
#include <iqt2d/CCircleParamsGuiComp.h>
#include <iqt2d/COrientedCircleEditorComp.h>
#include <iqt2d/CAnnulusParamsGuiComp.h>
#include <iqt2d/CAnnulusSegmentParamsGuiComp.h>
#include <iqt2d/CQuadrangleParamsGuiComp.h>
#include <iqt2d/CPolygonParamsGuiComp.h>
#include <iqt2d/CSceneBinderComp.h>
#include <iqt2d/CSelectableSceneExtenderComp.h>
#include <iqt2d/CViewProviderGuiComp.h>
#include <iqt2d/CExtendedConsoleGuiComp.h>
#include <iqt2d/CPolylineParamsGuiComp.h>
#include <iqt2d/CTubePolylineParamsGuiComp.h>
#include <iqt2d/CAffineTransformationParamsGuiComp.h>
#include <iqt2d/CMultiBitmapViewComp.h>
#include <iqt2d/CArcParamsGuiComp.h>


/**
	Qt scene visualization package.
*/
namespace QtViewPck
{


typedef icomp::TModelCompWrap<iview::CShapeStatusInfoComp> ShapeStatusInfo;
typedef iview::CShapeInfoGuiComp ShapeInfoGui;
typedef iview::CSimpleShapeFactoryComp SimpleShapeFactory;
typedef iview::CColorSchemaComp ColorSchema;
typedef iqt2d::CViewExtenderDelegatorComp SceneExtenderDelegator;
typedef iqt2d::CMultiViewExtenderDelegatorComp MultiSceneExtenderDelegator;
typedef iqt2d::CSceneConnectorGuiComp SceneConnector;
typedef ibase::TModelObserverCompWrap<
			icomp::TModelCompWrap<iqt2d::CImageViewComp> > ImageView;
typedef iqt2d::CPosition2dParamsGuiComp Position2dParamsGui;
typedef iqt2d::CLine2dParamsGuiComp Line2dParamsGui;
typedef iqt2d::CRectangleParamsGuiComp RectangleParamsGui;
typedef iqt2d::CCircleParamsGuiComp CircleParamsGui;
typedef iqt2d::COrientedCircleEditorComp OrientedCircleEditor;
typedef iqt2d::CAnnulusParamsGuiComp AnnulusParamsGui;
typedef iqt2d::CAnnulusSegmentParamsGuiComp AnnulusSegmentParamsGui;
typedef iqt2d::CQuadrangleParamsGuiComp QuadrangleParamsGui;
typedef iqt2d::CPolygonParamsGuiComp PolygonParamsGui;
typedef iqt2d::CSceneBinderComp SceneBinder;
typedef iqt2d::CSelectableSceneExtenderComp SelectableSceneExtender;
typedef icomp::TModelCompWrap<iqt2d::CViewProviderGuiComp> ViewProvider;
typedef iqt2d::CExtendedConsoleGuiComp ExtendedConsoleGui;
typedef iqt2d::CPolylineParamsGuiComp PolylineParamsGui;
typedef iqt2d::CTubePolylineParamsGuiComp TubePolylineParamsGui;
typedef iqt2d::CAffineTransformationParamsGuiComp AffineTransformationParamsGui;
typedef ibase::TModelObserverCompWrap<iqt2d::CMultiBitmapViewComp> MultiBitmapView;
typedef iqt2d::CArcParamsGuiComp ArcParamsGui;


} // namespace QtViewPck


#endif // !QtViewPck_included


