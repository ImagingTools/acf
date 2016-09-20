#ifndef iqtgui_CTabContainerGuiComp_included
#define iqtgui_CTabContainerGuiComp_included


// ACF includes
#include <iqtgui/CMultiPageGuiCompBase.h>


namespace iqtgui
{


/**
	Component to create the tab widget, that shows the specified sub-UI-component in its tab page.
	You can control some settings of the tabs such icons, title, orientation or tab visualization.
	Optionally, you can specify the corner widget, which will be placed beside of the last tab.
*/
class CTabContainerGuiComp: public CMultiPageGuiCompBase
{
public:
	typedef CMultiPageGuiCompBase BaseClass;

	I_BEGIN_COMPONENT(CTabContainerGuiComp);
		I_ASSIGN_MULTI_0(m_slaveWidgetsCompPtr, "Guis", "Slave widgets for tab window", true);
		I_ASSIGN(m_cornerGuiCompPtr, "CornerWidget", "Optional corner widget", false, "CornerWidget");
		I_ASSIGN(m_useTriangularTabsAttrPtr, "UseTriangularTabs", "Using triangular tab form", true, false);
		I_ASSIGN(m_tabOrientationAttrPtr, "TabBarOrientation", "Orientation of the tab bar\n 0 - North\n 1 - South\n 2 - West\n 3 - East", true, 0);
		I_ASSIGN(m_useDocumentModeAttrPtr, "UseDocumentMode", "If enabled, the tab widget will be switched into the document mode", true, false);
	I_END_COMPONENT;

protected:
	// reimplemented (iqtgui::CMultiPageGuiCompBase)
	virtual int GetPagesCount() const;
	virtual iqtgui::IGuiObject* GetPageGuiComponent(int pageIndex) const;
	virtual int GetDesignType() const;

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();

private:
	I_MULTIREF(IGuiObject, m_slaveWidgetsCompPtr);
	I_REF(iqtgui::IGuiObject, m_cornerGuiCompPtr);
	I_ATTR(bool, m_useTriangularTabsAttrPtr);
	I_ATTR(int, m_tabOrientationAttrPtr);
	I_ATTR(bool, m_useDocumentModeAttrPtr);
};


} // namespace iqtgui


#endif // !iqtgui_CTabContainerGuiComp_included

