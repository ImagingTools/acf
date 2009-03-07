#ifndef iqtgui_CTabContainerGuiComp_included
#define iqtgui_CTabContainerGuiComp_included


#include "iqtgui.h"


#include <QTabWidget>

#include "iqtgui/TGuiComponentBase.h"

#include "iqtgui/IIconProvider.h"


namespace iqtgui
{


class CTabContainerGuiComp: public iqtgui::TGuiComponentBase<QTabWidget> 
{
public:
	typedef iqtgui::TGuiComponentBase<QTabWidget> BaseClass;

	I_BEGIN_COMPONENT(CTabContainerGuiComp)
		I_ASSIGN_MULTI_0(m_slaveWidgetsCompPtr, "SlaveWidgets", "Slave widgets for tab window", true)
		I_ASSIGN(m_iconsProviderCompPtr, "IconsProvider", "Provider of tab icons", false, "IconsProvider")
		I_ASSIGN(m_cornerGuiCompPtr, "CornerWidget", "Optional corner widget", false, "CornerWidget")
		I_ASSIGN_MULTI_0(m_tabNamesAttrPtr, "TabTitles", "Titles for the tab", true)
		I_ASSIGN(m_iconSizeAttrPtr, "IconSize", "Size for tab icons", false, 16)
		I_ASSIGN(m_useTriangularTabsAttrPtr, "UseTriangularTabs", "Using triangular tab form", false, false)
		I_ASSIGN(m_tabOrientationAttrPtr, "TabBarOrientation", "Orientation of the tab bar", false, 0)
	I_END_COMPONENT

protected:
	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();

private:
	I_MULTIREF(iqtgui::IGuiObject, m_slaveWidgetsCompPtr);
	I_REF(iqtgui::IIconProvider, m_iconsProviderCompPtr);
	I_REF(iqtgui::IGuiObject, m_cornerGuiCompPtr);
	I_MULTIATTR(istd::CString, m_tabNamesAttrPtr);
	I_ATTR(int, m_iconSizeAttrPtr);
	I_ATTR(bool, m_useTriangularTabsAttrPtr);
	I_ATTR(int, m_tabOrientationAttrPtr);
};


} // namespace iqtgui


#endif // !iqtgui_CTabContainerGuiComp_included

