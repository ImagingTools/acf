#ifndef iqtgui_CToolBarGuiCompBase_included
#define iqtgui_CToolBarGuiCompBase_included


// Qt includes
#include <QtCore/QMap>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QToolBar>
#else
#include <QtGui/QToolBar>
#endif

// ACF includes
#include "iqtgui/IMainWindowComponent.h"
#include "iqtgui/TGuiComponentBase.h"


namespace iqtgui
{


/**
	Basic implementation of a tool bar component.
*/
class CToolBarGuiCompBase:
			public iqtgui::TGuiComponentBase<QToolBar>,
			virtual public iqtgui::IMainWindowComponent
{
public:
	typedef iqtgui::TGuiComponentBase<QToolBar> BaseClass;

	I_BEGIN_COMPONENT(CToolBarGuiCompBase);
		I_REGISTER_INTERFACE(iqtgui::IMainWindowComponent);
		I_ASSIGN(m_toolBarAreaAttrPtr, "ToolBarArea", "Specify the area for this toolbar\n 0 - left\n 1 - right\n 2 - top\n 3 - bottom", false, 0);
		I_ASSIGN(m_useVerticalOrientationAttrPtr, "UseVerticalOrientation", "Use vertical orientation of the tool bar", true, false);
		I_ASSIGN(m_dockFeaturesAttrPtr, "DockFeatures", "Specify the dock features for this widget\nIt is combination of options:\n2 - moveable\n4 - floatable", true, WCF_MOVEABLE | WCF_FLOATABLE);
		I_ASSIGN(m_allowedDockAreasAttrPtr, "AllowedDockAreas", "Specify the allowed dock areas\nIt is combination of options:\n 1 - left\n 2 - right\n 4 - top\n 8 - bottom", false, 0);
		I_ASSIGN(m_titleAttrPtr, "Title", "Tool bar title", false, "");
		I_ASSIGN(m_iconSizeAttrPtr, "IconSize", "Size of the icon used in the tool bar", false, 32);
		I_ASSIGN(m_buttonStyleAttrPtr, "ButtonStyle", "Button style used by the tool bar\n0 - Only display the icon\n1 - Only display the text\n2 - The text appears beside the icon\n3 - The text appears under the icon\n4 - Follow the system style", true, Qt::ToolButtonIconOnly);
	I_END_COMPONENT;

	// reimplemented (iqtgui::IMainWindowComponent)
	virtual bool AddToMainWindow(QMainWindow& mainWindow);
	virtual bool RemoveFromMainWindow(QMainWindow& mainWindow);
	virtual QString GetTitle() const;
	virtual int GetFlags() const;

protected:
	// reimplemented (CGuiComponentBase)
	virtual void OnGuiCreated();

private:
	I_ATTR(int, m_toolBarAreaAttrPtr);
	I_ATTR(bool, m_useVerticalOrientationAttrPtr);
	I_ATTR(int, m_dockFeaturesAttrPtr);
	I_ATTR(int, m_allowedDockAreasAttrPtr);
	I_ATTR(QString, m_titleAttrPtr);
	I_ATTR(int, m_iconSizeAttrPtr);
	I_ATTR(int, m_buttonStyleAttrPtr);
};


} // namespace iqtgui


#endif // !iqtgui_CToolBarGuiCompBase_included


