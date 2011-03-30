#ifndef iqtgui_CStatusBarWidgetGuiComp_included
#define iqtgui_CStatusBarWidgetGuiComp_included


// Qt includes
#include <QStatusBar>


// ACF includes
#include "icomp/CComponentBase.h"

#include "iqtgui/IMainWindowComponent.h"
#include "iqtgui/IGuiObject.h"


namespace iqtgui
{


/**
	Component for inserting of a widget into a status bar of the application.
*/
class CStatusBarWidgetComp:
			public icomp::CComponentBase,
			virtual public iqtgui::IMainWindowComponent
{
public:
	typedef icomp::CComponentBase BaseClass; 
	
	I_BEGIN_COMPONENT(CStatusBarWidgetComp);
		I_REGISTER_INTERFACE(iqtgui::IMainWindowComponent);
		I_ASSIGN(m_statusBarWidgetCompPtr, "StatusBarWidget", "Widget to place into the status bar of the application", true, "StatusBarWidget");
	I_END_COMPONENT;

	// reimplemented (iqtgui::IMainWindowComponent)
	virtual bool AddToMainWindow(QMainWindow& mainWindow);
	virtual bool RemoveFromMainWindow(QMainWindow& mainWindow);

private:
	I_REF(iqtgui::IGuiObject, m_statusBarWidgetCompPtr);
};


} // namespace iqtgui


#endif // !iqtgui_CStatusBarWidgetGuiComp_included


