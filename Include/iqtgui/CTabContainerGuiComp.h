#ifndef iqtgui_CTabContainerGuiComp_included
#define iqtgui_CTabContainerGuiComp_included


// Qt includes
#include <QtCore/QMap>
#include <QtGui/QTabWidget>

// ACF includes
#include "imod/CMultiModelDispatcherBase.h"

#include "iqtgui/TGuiComponentBase.h"


namespace iqtgui
{


/**
	Component to create the tab widget, that shows the specified sub-UI-component in its tab page.
	You can control some settings of the tabs such icons, title, orientation or tab visualization.
	Optionally, you can specify the corner widget, which will be placed beside of the last tab.
*/
class CTabContainerGuiComp:
			public iqtgui::TGuiComponentBase<QTabWidget>,
			protected imod::CMultiModelDispatcherBase
{
public:
	typedef iqtgui::TGuiComponentBase<QTabWidget> BaseClass;

	I_BEGIN_COMPONENT(CTabContainerGuiComp);
		I_ASSIGN_MULTI_0(m_slaveWidgetsCompPtr, "Guis", "Slave widgets for tab window", true);
		I_ASSIGN_TO(m_slaveWidgetsVisualCompPtr, m_slaveWidgetsCompPtr, false);
		I_ASSIGN_TO(m_slaveWidgetsModelCompPtr, m_slaveWidgetsCompPtr, false);
		I_ASSIGN(m_cornerGuiCompPtr, "CornerWidget", "Optional corner widget", false, "CornerWidget");
		I_ASSIGN_MULTI_0(m_tabNamesAttrPtr, "Names", "Titles for the tab", true);
		I_ASSIGN(m_iconSizeAttrPtr, "IconSize", "Size for tab icons", false, 16);
		I_ASSIGN(m_useTriangularTabsAttrPtr, "UseTriangularTabs", "Using triangular tab form", true, false);
		I_ASSIGN(m_tabOrientationAttrPtr, "TabBarOrientation", "Orientation of the tab bar\n 0 - North\n 1 - South\n 2 - West\n 3 - East", true, 0);
	I_END_COMPONENT;

protected:
	void UpdateVisualElements();

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();

	// reimplemented (imod::CMultiModelDispatcherBase)
	virtual void OnModelChanged(int modelId, int changeFlags, istd::IPolymorphic* updateParamsPtr);

private:
	I_MULTIREF(IGuiObject, m_slaveWidgetsCompPtr);
	I_MULTIREF(IVisualStatusProvider, m_slaveWidgetsVisualCompPtr);
	I_MULTIREF(imod::IModel, m_slaveWidgetsModelCompPtr);
	I_REF(iqtgui::IGuiObject, m_cornerGuiCompPtr);
	I_MULTIATTR(QString, m_tabNamesAttrPtr);
	I_ATTR(int, m_iconSizeAttrPtr);
	I_ATTR(bool, m_useTriangularTabsAttrPtr);
	I_ATTR(int, m_tabOrientationAttrPtr);

	typedef QMap<int, int> TabToGuiIndexMap;
	TabToGuiIndexMap m_tabToGuiIndexMap;
};


} // namespace iqtgui


#endif // !iqtgui_CTabContainerGuiComp_included

