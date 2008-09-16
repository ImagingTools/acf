#ifndef QtPck_included
#define QtPck_included


#include "icomp/TModelCompWrap.h"
#include "icomp/TMakeComponentWrap.h"

#include "iqt/CPackagesLoaderComp.h"
#include "iqt/CLoginGuiComp.h"
#include "iqt/CApplicationComp.h"
#include "iqt/CSplashScreenGuiComp.h"
#include "iqt/CLogGuiComp.h"
#include "iqt/CIconProviderComp.h" 
#include "iqt/CFileDialogSerializerComp.h"
#include "iqt/CBitmap.h"
#include "iqt/CDockWidgetGuiComp.h"
#include "iqt/CSelectionParamGuiComp.h"
#include "iqt/CTabContainerGuiComp.h"
#include "iqt/CSettingsSerializerComp.h"

#include "iqt2d/CSceneConnectorGuiComp.h"
#include "iqt2d/CSceneProviderComp.h"
#include "iqt2d/CImageViewComp.h"
#include "iqt2d/CLine2dParamsGuiComp.h"
#include "iqt2d/CRectangleParamsGuiComp.h"
#include "iqt2d/CCircleParamsGuiComp.h"
#include "iqt2d/CAnnulusParamsGuiComp.h"
#include "iqt2d/CAnnulusSegmentParamsGuiComp.h"

#include "iqtprm/CFileNameParamGuiComp.h"
#include "iqtprm/CComposedParamsSetGuiComp.h"
#include "iqtprm/CParamsManagerGuiComp.h"

#include "iqtdoc/CModelEditorGuiComp.h"
#include "iqtdoc/CExtendedDocumentTemplateComp.h"
#include "iqtdoc/CMainWindowGuiComp.h"
#include "iqtdoc/CMultiDocumentWorkspaceGuiComp.h"


/**
	Standard Qt package.
*/
namespace QtPck
{


typedef iqt::CPackagesLoaderComp PackagesLoader;
typedef iqt::CLoginGuiComp LoginGui;
typedef iqt::CApplicationComp GuiApplication;
typedef iqt::CSplashScreenGuiComp SplashScreen;
typedef icomp::TModelCompWrap<iqt::CLogGuiComp> Log;
typedef iqt::CIconProviderComp IconProvider;
typedef iqt::CFileDialogSerializerComp FileDialogSerializer;
typedef iqtdoc::CModelEditorGuiComp ModelEditorGui;
typedef iqtdoc::CExtendedDocumentTemplateComp ExtendedDocumentTemplate;
typedef iqtdoc::CMainWindowGuiComp MainWindowGui;
typedef icomp::TModelCompWrap<iqtdoc::CMultiDocumentWorkspaceGuiComp> MultiDocWorkspaceGui;
typedef icomp::TModelCompWrap<icomp::TMakeComponentWrap<iqt::CBitmap, iimg::IBitmap, iimg::IRasterImage, iser::ISerializable> > Bitmap;

typedef iqt2d::CSceneConnectorGuiComp SceneConnector;
typedef iqt2d::CSceneProviderComp SceneProvider;
typedef iqt2d::CImageViewComp ImageView;
typedef iqt2d::CLine2dParamsGuiComp Line2dParamsGui;
typedef iqt2d::CRectangleParamsGuiComp RectangleParamsGui;
typedef iqt2d::CCircleParamsGuiComp CircleParamsGui;
typedef iqt2d::CAnnulusParamsGuiComp AnnulusParamsGui;
typedef iqt2d::CAnnulusSegmentParamsGuiComp AnnulusSegmentParamsGui;

typedef iqtprm::CComposedParamsSetGuiComp ComposedParamsSetGui;
typedef iqtprm::CParamsManagerGuiComp ParamsManagerGui;
typedef iqtprm::CFileNameParamGuiComp FileNameParamGui;

typedef iqt::CDockWidgetGuiComp DockWidgetGui;
typedef iqt::CSelectionParamGuiComp SelectionParamGui;
typedef iqt::CTabContainerGuiComp TabContainer;
typedef iqt::CSettingsSerializerComp SettingsSerializer;


} // namespace QtPck


#endif // !QtPck_included


