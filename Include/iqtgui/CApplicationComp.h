#ifndef iqtgui_CApplicationComp_included
#define iqtgui_CApplicationComp_included


#include "ibase/IApplication.h"
#include "ibase/IApplicationInfo.h"

#include "icomp/TSingleAttribute.h"
#include "icomp/CComponentBase.h"

#include "iqtgui/IGuiObject.h"


namespace iqtgui
{


class CApplicationComp:
			public icomp::CComponentBase, 
			public ibase::IApplication
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CApplicationComp)
		I_REGISTER_INTERFACE(ibase::IApplication)
		I_ASSIGN(m_mainGuiCompPtr, "MainGui", "Gui object shown as main window", true, "MainGui")
		I_ASSIGN(m_splashScreenCompPtr, "SplashScreen", "Splash screen shown before application is launched", false, "SplashScreen")
		I_ASSIGN(m_applicationInfoCompPtr, "ApplicationInfo", "Application info used to set main window title", false, "ApplicationInfo")
		I_ASSIGN(m_freeComponentCompPtr, "FreeComponent", "Free component initialization", false, "FreeComponent")
		I_ASSIGN(m_splashTimeAttrPtr, "SplashTime", "Minimal time splash screen will be shown", true, 2)
		I_ASSIGN(m_styleSheetAttrPtr, "StyleSheet", "Style sheet for the GUI", false, "")
	I_END_COMPONENT

	bool EnsureInitialized(int argc, char** argv);

	// reimplemented (ibase::IApplication)
	virtual int Execute(int argc, char** argv);
	virtual istd::CString GetHelpText() const;

private:
	void SetStyleSheet(const QString& styleSheetFileName);

private:
	I_REF(IGuiObject, m_mainGuiCompPtr);
	I_REF(IGuiObject, m_splashScreenCompPtr);
	I_REF(ibase::IApplicationInfo, m_applicationInfoCompPtr);
	I_REF(void*, m_freeComponentCompPtr);
	I_ATTR(double, m_splashTimeAttrPtr);
	I_ATTR(istd::CString, m_styleSheetAttrPtr);

	istd::TDelPtr<QApplication> m_applicationPtr;
};


} // namespace iqtgui


#endif // iqtgui_CApplicationComp_included

