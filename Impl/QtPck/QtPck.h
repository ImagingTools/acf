#ifndef QtPck_included
#define QtPck_included


#include "icomp/TModelCompWrap.h"
#include "icomp/TMakeComponentWrap.h"

#include "iqt/CPackagesLoaderComp.h"
#include "iqt/CBitmap.h"
#include "iqt/CBitmapSurface.h"
#include "iqt/CBitmapLoaderComp.h"
#include "iqt/CSettingsSerializerComp.h"
#include "iqt/CApplicationSettingsProviderComp.h"
#include "iqt/CIniSettingsProviderComp.h"
#include "iqt/CFileInfoCopyComp.h"
#include "iqt/CCopyProcessorComp.h"
#include "iqt/CFileListProviderComp.h"
#include "iqt/CClipboardSerializerComp.h"

#include "iqtgui/CLoginGuiComp.h"
#include "iqtgui/CApplicationComp.h"
#include "iqtgui/CSplashScreenGuiComp.h"
#include "iqtgui/CComposedGuiComp.h"
#include "iqtgui/CLogGuiComp.h"
#include "iqtgui/CIconProviderComp.h" 
#include "iqtgui/CFileDialogLoaderComp.h"
#include "iqtgui/CDockWidgetGuiComp.h"
#include "iqtgui/CToolBarWidgetGuiComp.h"
#include "iqtgui/CFreeWidgetGuiComp.h"
#include "iqtgui/CSelectionParamGuiComp.h"
#include "iqtgui/CTabContainerGuiComp.h"
#include "iqtgui/CAboutWidgetGuiComp.h"
#include "iqtgui/CTabbedMultiViewGuiComp.h"
#include "iqtgui/CLayoutGuiComp.h"

#include "iqt2d/CSceneConnectorGuiComp.h"
#include "iqt2d/CSceneProviderGuiComp.h"
#include "iqt2d/CImageViewComp.h"
#include "iqt2d/CLine2dParamsGuiComp.h"
#include "iqt2d/CRectangleParamsGuiComp.h"
#include "iqt2d/CCircleParamsGuiComp.h"
#include "iqt2d/CAnnulusParamsGuiComp.h"
#include "iqt2d/CAnnulusSegmentParamsGuiComp.h"
#include "iqt2d/CQuadrangleParamsGuiComp.h"
#include "iqt2d/CSceneControllerGuiComp.h"

#include "iqt3d/CSurfaceViewComp.h"

#include "iqtprm/CFileNameParamGuiComp.h"
#include "iqtprm/CComposedParamsSetGuiComp.h"
#include "iqtprm/CParamsManagerGuiComp.h"

#include "iqtdoc/CModelEditorGuiComp.h"
#include "iqtdoc/CExtendedDocumentTemplateComp.h"
#include "iqtdoc/CMainWindowGuiComp.h"
#include "iqtdoc/CSingleDocumentWorkspaceGuiComp.h"
#include "iqtdoc/CMultiDocumentWorkspaceGuiComp.h"
#include "iqtdoc/CHtmlHelpViewerComp.h"
#include "iqtdoc/CDoxygenHelpFileProviderComp.h"


/**
	Standard Qt package.
*/
namespace QtPck
{


typedef iqt::CPackagesLoaderComp PackagesLoader;
typedef icomp::TModelCompWrap<
			icomp::TMakeComponentWrap<
						iqt::CBitmap,
						iimg::IBitmap,
						iimg::IRasterImage,
						iser::ISerializable> > Bitmap;
typedef icomp::TModelCompWrap<
			icomp::TMakeComponentWrap<
						iqt::CBitmapSurface,
						iimg::IBitmap,
						iimg::IRasterImage,
						iser::ISerializable,
						imath::ISampledFunction2d> > BitmapSurface;
typedef iqt::CBitmapLoaderComp BitmapLoader;
typedef iqt::CSettingsSerializerComp SettingsSerializer;
typedef iqt::CApplicationSettingsProviderComp ApplicationSettingsProvider;
typedef iqt::CIniSettingsProviderComp IniSettingsProvider;
typedef iqt::CFileInfoCopyComp FileInfoCopy;
typedef iqt::CCopyProcessorComp CopyProcessor;
typedef icomp::TModelCompWrap<iqt::CFileListProviderComp> FileListProvider;
typedef iqt::CClipboardSerializerComp ClipboardSerializer;

typedef iqtgui::CLoginGuiComp LoginGui;
typedef iqtgui::CApplicationComp GuiApplication;
typedef iqtgui::CSplashScreenGuiComp SplashScreen;
typedef icomp::TModelCompWrap<iqtgui::CLogGuiComp> Log;
typedef iqtgui::CComposedGuiComp ComposedGui;
typedef iqtgui::CIconProviderComp IconProvider;
typedef iqtgui::CFileDialogLoaderComp FileDialogLoader;
typedef iqtgui::CDockWidgetGuiComp DockWidgetGui;
typedef iqtgui::CToolBarWidgetGuiComp ToolBarWidgetGui;
typedef iqtgui::CFreeWidgetGuiComp FreeWidgetGui;
typedef iqtgui::CSelectionParamGuiComp SelectionParamGui;
typedef iqtgui::CTabContainerGuiComp TabContainer;
typedef iqtgui::CAboutWidgetGuiComp AboutGui;
typedef iqtgui::CTabbedMultiViewGuiComp TabbedMultiViewGui;
typedef iqtgui::CLayoutGuiComp LayoutGui;

typedef iqt2d::CSceneConnectorGuiComp SceneConnector;
typedef iqt2d::CSceneProviderGuiComp SceneProvider;
typedef iqt2d::CImageViewComp ImageView;
typedef iqt2d::CLine2dParamsGuiComp Line2dParamsGui;
typedef iqt2d::CRectangleParamsGuiComp RectangleParamsGui;
typedef iqt2d::CCircleParamsGuiComp CircleParamsGui;
typedef iqt2d::CAnnulusParamsGuiComp AnnulusParamsGui;
typedef iqt2d::CAnnulusSegmentParamsGuiComp AnnulusSegmentParamsGui;
typedef iqt2d::CQuadrangleParamsGuiComp QuadrangleParamsGui;
typedef iqt2d::CSceneControllerGuiComp SceneControllerGui;

typedef iqt3d::CSurfaceViewComp SurfaceView;

typedef iqtprm::CComposedParamsSetGuiComp ComposedParamsSetGui;
typedef iqtprm::CParamsManagerGuiComp ParamsManagerGui;
typedef iqtprm::CFileNameParamGuiComp FileNameParamGui;

typedef iqtdoc::CModelEditorGuiComp ModelEditorGui;
typedef iqtdoc::CExtendedDocumentTemplateComp ExtendedDocumentTemplate;
typedef iqtdoc::CMainWindowGuiComp MainWindowGui;
typedef icomp::TModelCompWrap<iqtdoc::CSingleDocumentWorkspaceGuiComp> SingleDocWorkspaceGui;
typedef icomp::TModelCompWrap<iqtdoc::CMultiDocumentWorkspaceGuiComp> MultiDocWorkspaceGui;
typedef iqtdoc::CHtmlHelpViewerComp HtmlHelpViewer;
typedef iqtdoc::CDoxygenHelpFileProviderComp DoxygenHelpFileProvider;


} // namespace QtPck


#endif // !QtPck_included


