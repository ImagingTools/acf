#ifndef iqtgui_CSplashScreen_included
#define iqtgui_CSplashScreen_included


#include <QSplashScreen>

#include "ibase/IApplicationInfo.h"

#include "icomp/TSingleAttribute.h"

#include "iqtgui/TDesignerGuiCompBase.h"

#include "iqtgui/Generated/ui_CSplashScreenGuiComp.h"


namespace iqtgui
{


/**
	Splash screen component.
	This component allows to create splash screen with additionally information about version, copyright an so on.
*/
class CSplashScreenGuiComp: public TDesignerGuiCompBase<Ui::CSplashScreenGuiComp, QSplashScreen>
{
public:
	typedef TDesignerGuiCompBase<Ui::CSplashScreenGuiComp, QSplashScreen> BaseClass;

	I_BEGIN_COMPONENT(CSplashScreenGuiComp);
		I_ASSIGN(m_applicationInfoCompPtr, "ApplicationInfo", "Version management for application", false, "Application");
		I_ASSIGN(m_imagePathAttrPtr, "ImagePath", "Path of image shown as splash screen", true, "Splash.jpg");
		I_ASSIGN(m_productNameAttrPtr, "ProductName", "Optional product name if is not painted at image", false, "ACF Product");
		I_ASSIGN(m_productTypeAttrPtr, "ProductType", "Additional product description shown over version info", false, "Server");
		I_ASSIGN(m_copyrightTextAttrPtr, "CopyrightText", "Optional copyright text", false, "(C) Copyright 2008 CompanyName\n\nAll rights reserved");
		I_ASSIGN_MULTI_1(m_versionIdsAttrPtr, "VersionIds", "List of version ID's for version infor should be displayed", false, 0);
		I_ASSIGN_MULTI_1(m_versionNamesAttrPtr, "VersionNames", "List of version names will be used as version description", false, "Version");
	I_END_COMPONENT;

protected:
	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();

private:
	I_REF(ibase::IApplicationInfo, m_applicationInfoCompPtr);
	I_ATTR(istd::CString, m_imagePathAttrPtr);
	I_ATTR(istd::CString, m_productNameAttrPtr);
	I_ATTR(istd::CString, m_productTypeAttrPtr);
	I_ATTR(istd::CString, m_copyrightTextAttrPtr);
	I_MULTIATTR(int, m_versionIdsAttrPtr);
	I_MULTIATTR(istd::CString, m_versionNamesAttrPtr);
};


} // namespace iqtgui


#endif // iqtgui_CSplashScreen_included


