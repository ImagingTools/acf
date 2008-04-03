#ifndef iqt_CSplashScreen_included
#define iqt_CSplashScreen_included


#include <QSplashScreen>

#include "ibase/IApplicationInfo.h"

#include "icomp/TSingleAttribute.h"

#include "iqt/TDesignerGuiComponentBase.h"

#include "iqt/Generated/ui_CSplashScreenGuiComp.h"


namespace iqt
{


class CSplashScreenGuiComp: public TDesignerGuiComponentBase<Ui::CSplashScreenGuiComp, QSplashScreen>
{
public:
	typedef TDesignerGuiComponentBase<Ui::CSplashScreenGuiComp, QSplashScreen> BaseClass;
	I_BEGIN_COMPONENT(CSplashScreenGuiComp)
		I_ASSIGN(m_applicationInfoCompPtr, "ApplicationInfo", "Version management for application", "Application", false)
		I_ASSIGN(m_imagePathAttrPtr, "ImagePath", "Path of image shown as splash screen", "Splash-jpg", true)
		I_ASSIGN(m_productTypeAttrPtr, "ProductType", "Additional product description shown over version info", "Server", false)
		I_ASSIGN(m_showAcfVersionAttrPtr, "ShowAcfVersion", "If true, ACF version will be shown", true, true)
		I_ASSIGN(m_showProductVersionAttrPtr, "ShowProductVersion", "If true, product version will be shown", true, true)
		I_ASSIGN(m_copyrightTextAttrPtr, "CopyrightText", "Optional copyright text", "(C) Copyright 2008 CompanyName\n\nAll rights reseved", false)
	I_END_COMPONENT

	// reimplemented (iqt::CGuiComponentBase)
	virtual void OnGuiShown();

private:
	I_REF(ibase::IApplicationInfo, m_applicationInfoCompPtr);
	I_ATTR(istd::CString, m_imagePathAttrPtr);
	I_ATTR(istd::CString, m_productTypeAttrPtr);
	I_ATTR(bool, m_showAcfVersionAttrPtr);
	I_ATTR(bool, m_showProductVersionAttrPtr);
	I_ATTR(istd::CString, m_copyrightTextAttrPtr);
};


} // namespace iqt


#endif // iqt_CSplashScreen_included


