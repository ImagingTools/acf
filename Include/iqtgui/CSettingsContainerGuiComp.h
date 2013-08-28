#ifndef iqtgui_CSettingsContainerGuiComp_included
#define iqtgui_CSettingsContainerGuiComp_included


// Qt includes
#include <QtCore/QtGlobal>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QListWidget>
#else
#include <QtGui/QStackedWidget>
#include <QtGui/QListWidget>
#endif

// ACF includes
#include "iqtgui/CContainerGuiBase.h"


namespace iqtgui
{


/**
	GUI container providing vertical menu for selection between the child UIs.
	Typically used for building the settings dialogs.
*/
class CSettingsContainerGuiComp:
				public iqtgui::TContainerGuiBase<QWidget>
{
	Q_OBJECT
public:
	typedef iqtgui::TContainerGuiBase<QWidget> BaseClass;

	I_BEGIN_COMPONENT(CSettingsContainerGuiComp);
		I_ASSIGN(m_fixedWidthAttrPtr, "FixedWidth", "Fixed width of the buttons", false, 100);
		I_ASSIGN(m_fixedHeightAttrPtr, "FixedHeight", "Fixed height of the buttons", false, 32);
		I_ASSIGN(m_alternateColorsAttrPtr, "AlternateColors", "Alternate colors for each button", true, true);
		I_ASSIGN(m_menuPositionAttrPtr, "SelectorPosition", "0 - Left\n1 - Right", true, 0);
	I_END_COMPONENT;

protected:
	// reimplemented (CContainerGuiBase)
	virtual bool CreateMainGui(QSize iconSize);
	virtual int CreatePageGui(const QString& name, QSize iconSize, iqtgui::IGuiObject* guiPtr);
	virtual void UpdateItem(int index, const QIcon& icon, const QString& toolTip);
	virtual bool SetSelectedIndex(int index);
	virtual bool SetEnabled(int index, bool isEnabled);

private:
	I_ATTR(int, m_fixedWidthAttrPtr);
	I_ATTR(int, m_fixedHeightAttrPtr);
	I_ATTR(bool, m_alternateColorsAttrPtr);
	I_ATTR(int, m_menuPositionAttrPtr);

	QStackedWidget* m_stackedWidgetPtr;
	QListWidget* m_menuListPtr;
};


} // namespace iqtgui


#endif // !iqtgui_CSettingsContainerGuiComp_included
