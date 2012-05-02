#ifndef QtViewPck_included
#define QtViewPck_included


#include "icomp/TModelCompWrap.h"
#include "icomp/TMakeComponentWrap.h"

#include "iqt2d/TViewExtenderDelegatorCompWrap.h"
#include "iqt2d/CSceneConnectorGuiComp.h"
#include "iqt2d/CImageViewComp.h"
#include "iqt2d/CLine2dParamsGuiComp.h"
#include "iqt2d/CRectangleParamsGuiComp.h"
#include "iqt2d/CCircleParamsGuiComp.h"
#include "iqt2d/CAnnulusParamsGuiComp.h"
#include "iqt2d/CAnnulusSegmentParamsGuiComp.h"
#include "iqt2d/CQuadrangleParamsGuiComp.h"
#include "iqt2d/CPolygonParamsGuiComp.h"
#include "iqt2d/CSceneBinderComp.h"
#include "iqt2d/CSelectableSceneExtenderComp.h"
#include "iqt2d/CViewProviderGuiComp.h"


/**
	Qt scene visualization package.
*/
namespace QtViewPck
{


typedef iqt2d::CViewExtenderDelegatorComp SceneExtenderDelegator;
typedef iqt2d::CSceneConnectorGuiComp SceneConnector;
typedef icomp::TModelCompWrap<iqt2d::CImageViewComp> ImageView;
typedef iqt2d::CLine2dParamsGuiComp Line2dParamsGui;
typedef iqt2d::CRectangleParamsGuiComp RectangleParamsGui;
typedef iqt2d::CCircleParamsGuiComp CircleParamsGui;
typedef iqt2d::CAnnulusParamsGuiComp AnnulusParamsGui;
typedef iqt2d::CAnnulusSegmentParamsGuiComp AnnulusSegmentParamsGui;
typedef iqt2d::CQuadrangleParamsGuiComp QuadrangleParamsGui;
typedef iqt2d::CPolygonParamsGuiComp PolygonParamsGui;
typedef iqt2d::CSceneBinderComp SceneBinder;
typedef iqt2d::CSelectableSceneExtenderComp SelectableSceneExtender;
typedef icomp::TModelCompWrap<iqt2d::CViewProviderGuiComp> ViewProvider;


} // namespace QtViewPck


#endif // !QtViewPck_included


