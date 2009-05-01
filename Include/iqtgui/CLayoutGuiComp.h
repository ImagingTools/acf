#ifndef iqtgui_CLayoutGuiComp_included
#define iqtgui_CLayoutGuiComp_included


#include "iqtgui.h"


#include <QTabWidget>

#include "iqtgui/TGuiComponentBase.h"

#include "iqtgui/IIconProvider.h"


namespace iqtgui
{


class CLayoutGuiComp: public iqtgui::TGuiComponentBase<QWidget> 
{
public:
	typedef iqtgui::TGuiComponentBase<QWidget> BaseClass;

	I_BEGIN_COMPONENT(CLayoutGuiComp)
		I_ASSIGN_MULTI_0(m_slaveWidgetsCompPtr, "SlaveWidgets", "Slave widgets for tab window", true)
		I_ASSIGN(m_layoutOrientationAttrPtr, "LayoutOrientation", "Orientation of the layout\n0 - Horizontaly\n1 - Verticaly", false, 0)
	I_END_COMPONENT

protected:
	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();

private:
	I_MULTIREF(iqtgui::IGuiObject, m_slaveWidgetsCompPtr);
	I_ATTR(int, m_layoutOrientationAttrPtr);
};


} // namespace iqtgui


#endif // !iqtgui_CLayoutGuiComp_included

