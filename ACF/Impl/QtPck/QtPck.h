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
#include "iqt/CFileAcquisitionParamsGuiComp.h"
#include "iqt/CSelectionParamGuiComp.h"
#include "iqt/CTabContainerGuiComp.h"
#include "iqt/CFileNameProviderGuiComp.h"
#include "iqt/CSettingsSerializerComp.h"

#include "iqt2d/CSceneConnectorGuiComp.h"
#include "iqt2d/CSceneProviderComp.h"
#include "iqt2d/CImageViewComp.h"
#include "iqt2d/CLine2dParamsGuiComp.h"

#include "iqtprm/CComposedParamsSetGuiComp.h"
#include "iqtprm/CParamsManagerGuiComp.h"

#include "iqtdoc/CModelEditorGuiComp.h"
#include "iqtdoc/CExtendedDocumentTemplateComp.h"
#include "iqtdoc/CMainWindowGuiComp.h"
#include "iqtdoc/CMultiDocumentWorkspaceGuiComp.h"

#include "iqtsig/CSamplingParamsGuiComp.h"
#include "iqtsig/CSampleAcquisitionGuiComp.h"
#include "iqtsig/CScriptSampleAcquisitionComp.h"


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
typedef iqtprm::CComposedParamsSetGuiComp ComposedParamsSetGui;
typedef iqtprm::CParamsManagerGuiComp ParamsManagerGui;
typedef iqt::CDockWidgetGuiComp DockWidgetGui;
typedef iqt::CFileAcquisitionParamsGuiComp FileAcquisitionParamsGui;
typedef iqt::CSelectionParamGuiComp SelectionParamGui;
typedef iqtsig::CSamplingParamsGuiComp SamplingParamsGui;
typedef iqtsig::CSampleAcquisitionGuiComp SampleAcquisitionGui;
typedef iqtsig::CScriptSampleAcquisitionComp ScriptSampleAcquisition;
typedef iqt::CTabContainerGuiComp TabContainer;
typedef iqt::CFileNameProviderGuiComp FileNameProviderGui;
typedef iqt::CSettingsSerializerComp SettingsSerializer;


} // namespace QtPck


#endif // !QtPck_included


