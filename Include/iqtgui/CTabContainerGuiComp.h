#ifndef iqtgui_CTabContainerGuiComp_included
#define iqtgui_CTabContainerGuiComp_included


// Qt includes
#include <QTabWidget>


// ACF includes
#include "iqtgui/TGuiComponentBase.h"

#include "iqtgui/IIconProvider.h"


namespace iqtgui
{


/**
	Component to create the tab widget, that shows the specified sub-UI-component in its tab page.
	You can control some settings of the tabs such icons, title, orientation or tab visualization.
	Optionally, you can specify the corner widget, which will be placed beside of the last tab.
*/
class CTabContainerGuiComp: public iqtgui::TGuiComponentBase<QTabWidget> 
{
public:
	typedef iqtgui::TGuiComponentBase<QTabWidget> BaseClass;

	I_BEGIN_COMPONENT(CTabContainerGuiComp);
		I_ASSIGN_MULTI_0(m_slaveWidgetsCompPtr, "Guis", "Slave widgets for tab window", true);
		I_ASSIGN(m_iconsProviderCompPtr, "IconsProvider", "Provider of tab icons", false, "IconsProvider");
		I_ASSIGN(m_cornerGuiCompPtr, "CornerWidget", "Optional corner widget", false, "CornerWidget");
		I_ASSIGN_MULTI_0(m_tabNamesAttrPtr, "Names", "Titles for the tab", true);
		I_ASSIGN(m_iconSizeAttrPtr, "IconSize", "Size for tab icons", false, 16);
		I_ASSIGN(m_useTriangularTabsAttrPtr, "UseTriangularTabs", "Using triangular tab form", true, false);
		I_ASSIGN(m_tabOrientationAttrPtr, "TabBarOrientation", "Orientation of the tab bar\n 0 - North\n 1 - South\n 2 - West\n 3 - East", true, 0);
	I_END_COMPONENT;

protected:
	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();

private:
	I_MULTIREF(iqtgui::IGuiObject, m_slaveWidgetsCompPtr);
	I_REF(iqtgui::IIconProvider, m_iconsProviderCompPtr);
	I_REF(iqtgui::IGuiObject, m_cornerGuiCompPtr);
	I_MULTIATTR(QString, m_tabNamesAttrPtr);
	I_ATTR(int, m_iconSizeAttrPtr);
	I_ATTR(bool, m_useTriangularTabsAttrPtr);
	I_ATTR(int, m_tabOrientationAttrPtr);
};


} // namespace iqtgui


#endif // !iqtgui_CTabContainerGuiComp_included

