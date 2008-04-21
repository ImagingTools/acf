#include "icomp/TModelCompWrap.h"
#include "icomp/TMakeComponentWrap.h"
#include "icomp/TSimComponentWrap.h"
#include "icomp/TSimComponentsFactory.h"

#include "istdc/TSingleFactoryComp.h"
#include "istdc/TFileSerializerComp.h"

#include "iqt/CApplicationComp.h"
#include "iqt/CSplashScreenGuiComp.h"
#include "iqt/CBitmapLoaderComp.h"

#include "iqmain/CExtendedDocumentTemplateComp.h"
#include "iqmain/CMainWindowGuiComp.h"
#include "iqmain/CMultiDocumentWorkspaceGuiComp.h"

#include "iqt/CBitmap.h"
#include "iqt2d/CImageViewComp.h"


int main(int argc, char *argv[])
{
	Q_INIT_RESOURCE(iqmain);

	icomp::TSimComponentWrap<iqt::CApplicationComp> application;
	application.EnsureInitialized(argc, argv);

	icomp::TSimComponentWrap<iqt::CSplashScreenGuiComp> splashScreenGui;
	splashScreenGui.SetStringAttr("ImagePath", "../../../Docs/Images/AcfSplashScreen.png");
	splashScreenGui.SetStringAttr("ProductName", "ImageView");
	splashScreenGui.SetStringAttr("ProductType", "Tutorial");
	splashScreenGui.SetStringAttr("CopyrightText", "This is a part of ACF project.\nSee 'licence.txt' for copyright informations");
	splashScreenGui.InitComponent();

	icomp::TSimComponentsFactory<icomp::TModelCompWrap<icomp::TMakeComponentWrap<imod::IModel, iqt::CBitmap> > > modelFactoryComp;
	icomp::TSimComponentsFactory<iqt2d::CImageViewComp> viewFactoryComp;

	icomp::TSimComponentWrap<iqt::CBitmapLoaderComp> bitmapLoaderComp;
	bitmapLoaderComp.InitComponent();

	icomp::TSimComponentWrap<iqmain::CExtendedDocumentTemplateComp> documentTemplateComp;
	documentTemplateComp.SetRef("AboutGui", &splashScreenGui);
	documentTemplateComp.SetFactory("DocumentFactory", &modelFactoryComp);
	documentTemplateComp.SetFactory("ViewFactory", &viewFactoryComp);
	documentTemplateComp.SetRef("DocumentLoader", &bitmapLoaderComp);
	documentTemplateComp.InsertMultiAttr("FileFilters", istd::CString("Bitmap files (*.bmp;*.png;*.jpg)"));
	documentTemplateComp.InsertMultiAttr("FileExtensions", istd::CString("bmp"));
	documentTemplateComp.InsertMultiAttr("FileExtensions", istd::CString("png"));
	documentTemplateComp.InsertMultiAttr("FileExtensions", istd::CString("jpg"));
	documentTemplateComp.InitComponent();

	icomp::TSimComponentWrap<icomp::TModelCompWrap<iqmain::CMultiDocumentWorkspaceGuiComp> > workspaceComp;
	workspaceComp.SetRef("DocumentTemplate", &documentTemplateComp);
	workspaceComp.InitComponent();

	icomp::TSimComponentWrap<iqmain::CMainWindowGuiComp> mainWindowComp;
	mainWindowComp.SetRef("Workspace", &workspaceComp);
	mainWindowComp.SetRef("DocumentManager", &workspaceComp);
	mainWindowComp.InitComponent();

	application.SetRef("MainGui", &mainWindowComp);
	application.SetRef("SplashScreen", &splashScreenGui);
	application.InitComponent();

	return application.Execute(argc, argv);
}

