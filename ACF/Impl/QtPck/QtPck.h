#ifndef QtPck_included
#define QtPck_included


#include "icomp/TModelCompWrap.h"
#include "icomp/TMakeComponentWrap.h"

#include "iqt/CLoginGuiComp.h"
#include "iqt/CApplicationComp.h"
#include "iqt/CSplashScreenGuiComp.h"
#include "iqt/CLogGuiComp.h"
#include "iqt/CIconProviderComp.h" 
#include "iqt/CFileDialogSerializerComp.h"
#include "iqt/CBitmap.h"
#include "iqt/CBitmapLoaderComp.h"
#include "iqt/CDockWidgetGuiComp.h"

#include "iqt2d/CImageViewComp.h"

#include "iqtdoc/CExtendedDocumentTemplateComp.h"
#include "iqtdoc/CMainWindowGuiComp.h"
#include "iqtdoc/CMultiDocumentWorkspaceGuiComp.h"


/**
	Standard Qt package.
*/
namespace QtPck
{


typedef iqt::CLoginGuiComp LoginGui;
typedef iqt::CApplicationComp GuiApplication;
typedef iqt::CSplashScreenGuiComp SplashScreen;
typedef iqt::CLogGuiComp Log;
typedef iqt::CIconProviderComp IconProvider;
typedef iqt::CFileDialogSerializerComp FileDialogSerializer;
typedef iqtdoc::CExtendedDocumentTemplateComp ExtendedDocumentTemplate;
typedef iqtdoc::CMainWindowGuiComp MainWindowGui;
typedef icomp::TModelCompWrap<iqtdoc::CMultiDocumentWorkspaceGuiComp> MultiDocWorkspaceGui;
typedef icomp::TModelCompWrap<icomp::TMakeComponentWrap<iimg::IBitmap, iqt::CBitmap> > Bitmap;
typedef iqt2d::CImageViewComp ImageView;
typedef iqt::CBitmapLoaderComp BitmapLoader;
typedef iqt::CDockWidgetGuiComp DockWidgetGui;


} // namespace QtPck


#endif // !QtPck_included


