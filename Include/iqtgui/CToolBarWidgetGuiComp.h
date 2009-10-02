#ifndef CToolBarWidgetGuiComp_h_included
#define CToolBarWidgetGuiComp_h_included


// Qt includes
#include <QToolBar>
#include <QMap>


// ACF includes
#include "iqtgui/IMainWindowComponent.h"
#include "iqtgui/TGuiComponentBase.h"


namespace iqtgui
{


class CToolBarWidgetGuiComp: 
			public iqtgui::TGuiComponentBase<QToolBar>, 
			public iqtgui::IMainWindowComponent
{
	Q_OBJECT
public:
	typedef iqtgui::TGuiComponentBase<QToolBar> BaseClass; 
	
	I_BEGIN_COMPONENT(CToolBarWidgetGuiComp)
		I_REGISTER_INTERFACE(iqtgui::IMainWindowComponent)
		I_ASSIGN(m_toolbarWidgetCompPtr, "ToolBarWidget", "", true, "ToolBarWidget")
		I_ASSIGN(m_dockAreaAttrPtr, "ToolBarArea", "Specify the area for this toolbar\n 0 - left\n 1 - right\n 2 - top\n 3 - bottom", false, 0)
		I_ASSIGN(m_titleAttrPtr, "Title", "", false, "")
	I_END_COMPONENT


	// reimplemented (iqtgui::IMainWindowComponent)
	virtual bool AddToMainWindow(QMainWindow& mainWindow);

protected:
	// reimplemented (CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();

protected:
	I_REF(iqtgui::IGuiObject, m_toolbarWidgetCompPtr);
	I_ATTR(int, m_dockAreaAttrPtr);
	I_ATTR(istd::CString, m_titleAttrPtr);
};


} // namespace iqtgui

	
#endif // CToolBarWidgetGuiComp_h_included

