#ifndef iqtgui_CSplashScreen_included
#define iqtgui_CSplashScreen_included


#include <QSplashScreen>

#include "ibase/IApplicationInfo.h"

#include "icomp/TSingleAttribute.h"

#include "iqtgui/TDesignerGuiCompBase.h"

#include "iqtgui/Generated/ui_CSplashScreenGuiComp.h"


namespace iqtgui
{


class CSplashScreenGuiComp: public TDesignerGuiCompBase<Ui::CSplashScreenGuiComp, QSplashScreen>
{
public:
	typedef TDesignerGuiCompBase<Ui::CSplashScreenGuiComp, QSplashScreen> BaseClass;

	I_BEGIN_COMPONENT(CSplashScreenGuiComp)
		I_ASSIGN(m_applicationInfoCompPtr, "ApplicationInfo", "Version management for application", false, "Application")
		I_ASSIGN(m_imagePathAttrPtr, "ImagePath", "Path of image shown as splash screen", true, "Splash.jpg")
		I_ASSIGN(m_productNameAttrPtr, "ProductName", "Optional product name if is not painted at image", false, "ACF Product")
		I_ASSIGN(m_productTypeAttrPtr, "ProductType", "Additional product description shown over version info", false, "Server")
		I_ASSIGN(m_showAcfVersionAttrPtr, "ShowAcfVersion", "If true, ACF version will be shown", true, true)
		I_ASSIGN(m_showProductVersionAttrPtr, "ShowProductVersion", "If true, product version will be shown", true, true)
		I_ASSIGN(m_copyrightTextAttrPtr, "CopyrightText", "Optional copyright text", false, "(C) Copyright 2008 CompanyName\n\nAll rights reserved")
	I_END_COMPONENT

protected:
	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();

private:
	I_REF(ibase::IApplicationInfo, m_applicationInfoCompPtr);
	I_ATTR(istd::CString, m_imagePathAttrPtr);
	I_ATTR(istd::CString, m_productNameAttrPtr);
	I_ATTR(istd::CString, m_productTypeAttrPtr);
	I_ATTR(bool, m_showAcfVersionAttrPtr);
	I_ATTR(bool, m_showProductVersionAttrPtr);
	I_ATTR(istd::CString, m_copyrightTextAttrPtr);
};


} // namespace iqtgui


#endif // iqtgui_CSplashScreen_included


