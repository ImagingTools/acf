#ifndef iqtgui_CGuiApplicationComp_included
#define iqtgui_CGuiApplicationComp_included


// ACF includes
#include "iqtgui/IGuiObject.h"
#include "iqtgui/IGuiApplication.h"
#include "iqtgui/CApplicationCompBase.h"


namespace iqtgui
{


/**
	Standard component for a Qt based GUI application.
	You have several attributes to control the appearance and decoration of the application window,
	you can define the splash screen and set some information about your application.
*/
class CGuiApplicationComp:
			public CApplicationCompBase,
			virtual public iqtgui::IGuiApplication
{
public:
	typedef CApplicationCompBase BaseClass;

	I_BEGIN_COMPONENT(CGuiApplicationComp);
		I_REGISTER_INTERFACE(ibase::IApplication);
		I_ASSIGN(m_mainGuiCompPtr, "MainGui", "Gui object shown as main window", false, "MainGui");
		I_ASSIGN(m_frameSpaceSizeAttrPtr, "FrameSpaceSize", "Number of pixels will be added on the all window sides", false, 9);
		I_ASSIGN(m_uiStartModeAttrPtr, "UiStartMode", "UI mode by application start up.\n0 - normal\n1 - full screen\n2 - minimized\n3 - maximized", false, 0);
	I_END_COMPONENT;

	// reimplemented (ibase::IGuiApplication)
	virtual const iqtgui::IGuiObject* GetApplicationGui() const;

	// reimplemented (ibase::IApplication)
	virtual int Execute(int argc, char** argv);
	virtual QString GetHelpText() const;

private:
	I_REF(IGuiObject, m_mainGuiCompPtr);
	I_ATTR(int, m_frameSpaceSizeAttrPtr);
	I_ATTR(int, m_uiStartModeAttrPtr);

	istd::TDelPtr<QWidget> m_mainWidgetPtr;
};


} // namespace iqtgui


#endif // iqtgui_CGuiApplicationComp_included

