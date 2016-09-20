#ifndef iqtgui_CToolBarWidgetGuiComp_included
#define iqtgui_CToolBarWidgetGuiComp_included


// ACF includes
#include <iqtgui/CToolBarGuiCompBase.h>


namespace iqtgui
{


/**
	Component for construction of a tool bar with an inserted widget.
*/
class CToolBarWidgetGuiComp: public CToolBarGuiCompBase
{
public:
	typedef CToolBarGuiCompBase BaseClass;

	
	I_BEGIN_COMPONENT(CToolBarWidgetGuiComp);
		I_REGISTER_INTERFACE(iqtgui::IMainWindowComponent);
		I_ASSIGN(m_toolbarWidgetCompPtr, "ToolBarWidget", "Widget to place into the tool bar", true, "ToolBarWidget");
	I_END_COMPONENT;

protected:
	// reimplemented (CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();

private:
	I_REF(iqtgui::IGuiObject, m_toolbarWidgetCompPtr);
};


} // namespace iqtgui


#endif // !iqtgui_CToolBarWidgetGuiComp_included

