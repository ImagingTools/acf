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
#include "iqt/CBitmapLoaderComp.h"
#include "iqt/CDockWidgetGuiComp.h"
#include "iqt/CSnapImageGuiComp.h"
#include "iqt/CComposedParamsSetGuiComp.h"
#include "iqt/CFileAcquisitionParamsGuiComp.h"
#include "iqt/CParamsManagerGuiComp.h"
#include "iqt/CSelectionParamGuiComp.h"
#include "iqt/CTabContainerGuiComp.h"
#include "iqt/CFileNameProviderGuiComp.h"
#include "iqt/CSettingsSerializerComp.h"

#include "iqt2d/CImageViewComp.h"

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
typedef iqtdoc::CExtendedDocumentTemplateComp ExtendedDocumentTemplate;
typedef iqtdoc::CMainWindowGuiComp MainWindowGui;
typedef icomp::TModelCompWrap<iqtdoc::CMultiDocumentWorkspaceGuiComp> MultiDocWorkspaceGui;
typedef icomp::TModelCompWrap<icomp::TMakeComponentWrap<iimg::IBitmap, iqt::CBitmap> > Bitmap;
typedef iqt2d::CImageViewComp ImageView;
typedef iqt::CBitmapLoaderComp BitmapLoader;
typedef iqt::CDockWidgetGuiComp DockWidgetGui;
typedef iqt::CSnapImageGuiComp SnapImageGui;
typedef iqt::CComposedParamsSetGuiComp ComposedParamsSetGui;
typedef iqt::CFileAcquisitionParamsGuiComp FileAcquisitionParamsGui;
typedef iqt::CParamsManagerGuiComp ParamsManagerGui;
typedef iqt::CSelectionParamGuiComp SelectionParamGui;
typedef iqtsig::CSamplingParamsGuiComp SamplingParamsGui;
typedef iqtsig::CSampleAcquisitionGuiComp SampleAcquisitionGui;
typedef iqtsig::CScriptSampleAcquisitionComp ScriptSampleAcquisition;
typedef iqt::CTabContainerGuiComp TabContainer;
typedef iqt::CFileNameProviderGuiComp FileNameProviderGui;
typedef iqt::CSettingsSerializerComp SettingsSerializer;


} // namespace QtPck


#endif // !QtPck_included


