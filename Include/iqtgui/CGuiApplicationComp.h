#ifndef iqtgui_CGuiApplicationComp_included
#define iqtgui_CGuiApplicationComp_included


// ACF includes
#include "ibase/IApplicationInfo.h"

#include "icomp/TAttribute.h"
#include "icomp/CComponentBase.h"

#include "iqt/ITranslationManager.h"

#include "iqtgui/IGuiObject.h"
#include "iqtgui/IGuiApplication.h"


namespace iqtgui
{


/**
	Standard component for a Qt based GUI application.
	You have several attributes to control the appearance and decoration of the application window,
	you can define the splash screen and set some information about your application.
*/
class CGuiApplicationComp:
			public icomp::CComponentBase, 
			public iqtgui::IGuiApplication
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CGuiApplicationComp);
		I_REGISTER_INTERFACE(ibase::IApplication);
		I_ASSIGN(m_mainGuiCompPtr, "MainGui", "Gui object shown as main window", false, "MainGui");
		I_ASSIGN(m_translationManagerCompPtr, "TranslationManager", "Translation manager", false, "TranslationManager");
		I_ASSIGN(m_splashScreenCompPtr, "SplashScreen", "Splash screen shown before application is launched", false, "SplashScreen");
		I_ASSIGN(m_applicationInfoCompPtr, "ApplicationInfo", "Application info used to set main window title", false, "ApplicationInfo");
		I_ASSIGN(m_splashTimeAttrPtr, "SplashTime", "Minimal time splash screen will be shown", true, 2);
		I_ASSIGN(m_styleSheetAttrPtr, "StyleSheet", "Style sheet for the GUI", false, "");
		I_ASSIGN(m_iconPathAttrPtr, "IconPath", "file path for the application icon", true, ":/Icons/AcfLogo.svg");
		I_ASSIGN(m_titleFormatAttrPtr, "TitleFormat", "Describe format of title bar, use %1 as application name and %2 as company name", true, "%1");
		I_ASSIGN(m_frameSpaceSizeAttrPtr, "FrameSpaceSize", "Number of pixels will be added on the all window sides", false, 9);
		I_ASSIGN(m_uiStartModeAttrPtr, "UiStartMode", "UI mode by application start up.\n0 - normal\n1 - full screen\n2 - minimized\n3 - maximized", false, 0);
	I_END_COMPONENT;

	// reimplemented (ibase::IGuiApplication)
	virtual const iqtgui::IGuiObject* GetApplicationGui() const;

	// reimplemented (ibase::IApplication)
	virtual bool InitializeApplication(int argc, char** argv);
	virtual int Execute(int argc, char** argv);
	virtual istd::CString GetHelpText() const;

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

private:
	void SetStyleSheet(const QString& styleSheetFileName);

private:
	I_REF(IGuiObject, m_mainGuiCompPtr);
	I_REF(IGuiObject, m_splashScreenCompPtr);
	I_REF(ibase::IApplicationInfo, m_applicationInfoCompPtr);
	I_REF(iqt::ITranslationManager, m_translationManagerCompPtr);
	I_ATTR(double, m_splashTimeAttrPtr);
	I_ATTR(istd::CString, m_styleSheetAttrPtr);
	I_ATTR(istd::CString, m_iconPathAttrPtr);
	I_ATTR(istd::CString, m_titleFormatAttrPtr);
	I_ATTR(int, m_frameSpaceSizeAttrPtr);
	I_ATTR(int, m_uiStartModeAttrPtr);

	istd::TDelPtr<QApplication> m_applicationPtr;

	istd::TDelPtr<QWidget> m_mainWidgetPtr;
};


} // namespace iqtgui


#endif // iqtgui_CGuiApplicationComp_included

