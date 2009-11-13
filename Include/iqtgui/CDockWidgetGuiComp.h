#ifndef iqtgui_CDockWidgetGuiComp_included
#define iqtgui_CDockWidgetGuiComp_included


// Qt includes
#include <QDockWidget>


// ACF includes
#include "iqtgui/IMainWindowComponent.h"
#include "iqtgui/TGuiComponentBase.h"


namespace iqtgui
{


class CDockWidgetGuiComp: public iqtgui::TGuiComponentBase<QDockWidget>, public iqtgui::IMainWindowComponent
{
	Q_OBJECT
public:
	typedef iqtgui::TGuiComponentBase<QDockWidget> BaseClass;

	I_BEGIN_COMPONENT(CDockWidgetGuiComp);
		I_REGISTER_INTERFACE(iqtgui::IMainWindowComponent);
		I_ASSIGN(m_slaveGuiCompPtr, "SlaveGui", "Slave GUI for this dock widget", true, "SlaveGui");
		I_ASSIGN(m_dockAreaAttrPtr, "DockArea", "Specify the dock area for this widget\n 0 - left\n 1 - right\n 2 - top\n 3 - bottom", false, 0);
		I_ASSIGN(m_dockFeaturesAttrPtr, "DockFeatures", "Specify the dock features for this widget\nIt is combination of options:\n 1 - closeable\n 2 - moveable\n 4 - floatable\n 8 - vertical title", false, 0);
		I_ASSIGN(m_allowedDockAreasAttrPtr, "AllowedDockAreas", "Specify the allowed dock areas\nIt is combination of options:\n 1 - left\n 2 - right\n 4 - top\n 8 - bottom", false, 0);
		I_ASSIGN(m_dockTitleAttrPtr, "DockTitle", "Specify the dock title ", false, "DockTitle");
	I_END_COMPONENT;
	
	// reimplemented (iqtgui::IMainWindowComponent)
	virtual bool AddToMainWindow(QMainWindow& mainWindow);
	virtual bool RemoveFromMainWindow(QMainWindow& mainWindow);

protected:
	// reimplemented (CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();
	virtual void OnRetranslate();

protected:
	I_REF(iqtgui::IGuiObject, m_slaveGuiCompPtr);
	I_ATTR(int, m_dockAreaAttrPtr);
	I_ATTR(int, m_dockFeaturesAttrPtr);
	I_ATTR(int, m_allowedDockAreasAttrPtr);
	I_ATTR(istd::CString, m_dockTitleAttrPtr);
};


} // namespace iqtgui


#endif // !iqtgui_CDockWidgetGuiComp_included

