#ifndef iqtgui_CApplicationCompBase_included
#define iqtgui_CApplicationCompBase_included


// ACF includes
#include "icomp/CComponentBase.h"

#include "ibase/IApplication.h"
#include "ibase/IApplicationInfo.h"

#include "iqt/ITranslationManager.h"
#include "iqt/CTimer.h"

#include "iqtgui/IGuiObject.h"


namespace iqtgui
{


/**
	Standard component for a Qt based GUI application.
	You have several attributes to control the appearance and decoration of the application window,
	you can define the splash screen and set some information about your application.
*/
class CApplicationCompBase:
			public icomp::CComponentBase, 
			virtual public ibase::IApplication
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_BASE_COMPONENT(CApplicationCompBase);
		I_REGISTER_INTERFACE(ibase::IApplication);
		I_ASSIGN(m_translationManagerCompPtr, "TranslationManager", "Translation manager", false, "TranslationManager");
		I_ASSIGN(m_splashScreenCompPtr, "SplashScreen", "Splash screen shown before application is launched", false, "SplashScreen");
		I_ASSIGN(m_applicationInfoCompPtr, "ApplicationInfo", "Application info used to set main window title", false, "ApplicationInfo");
		I_ASSIGN(m_splashTimeAttrPtr, "SplashTime", "Minimal time splash screen will be shown", true, 2);
		I_ASSIGN(m_styleSheetAttrPtr, "StyleSheet", "Style sheet for the GUI", false, "");
		I_ASSIGN(m_iconPathAttrPtr, "IconPath", "file path for the application icon", true, ":/Icons/AcfLogo.svg");
		I_ASSIGN(m_titleFormatAttrPtr, "TitleFormat", "Describe format of title bar, use %1 as application name and %2 as company name", true, "%1");
	I_END_COMPONENT;

	// reimplemented (ibase::IApplication)
	virtual bool InitializeApplication(int argc, char** argv);

protected:
	QApplication* GetQtApplication() const;
	bool TryShowSplashScreen();
	void HideSplashScreen();

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

private:
	void SetStyleSheet(const QString& styleSheetFileName);

private:
	I_REF(IGuiObject, m_splashScreenCompPtr);
	I_REF(ibase::IApplicationInfo, m_applicationInfoCompPtr);
	I_REF(iqt::ITranslationManager, m_translationManagerCompPtr);
	I_ATTR(double, m_splashTimeAttrPtr);
	I_ATTR(QString, m_styleSheetAttrPtr);
	I_ATTR(QString, m_iconPathAttrPtr);
	I_ATTR(QString, m_titleFormatAttrPtr);

	bool m_useSplashScreen;
	iqt::CTimer m_splashScreenTimer;

	istd::TDelPtr<QApplication> m_applicationPtr;
};


} // namespace iqtgui


#endif // iqtgui_CApplicationCompBase_included

