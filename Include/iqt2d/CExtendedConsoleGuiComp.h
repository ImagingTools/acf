#ifndef iqt2d_CExtendedConsoleGuiComp_included
#define iqt2d_CExtendedConsoleGuiComp_included


// ACF includes
#include "ibase/ICommandsProvider.h"

#include "iqtgui/TDesignerGuiCompBase.h"

#include "GeneratedFiles/iqt2d/ui_CExtendedConsoleGuiComp.h"


namespace iqt2d
{


class CExtendedConsoleGuiComp: public iqtgui::TDesignerGuiCompBase<Ui::CExtendedConsoleGuiComp>
{
public:
	typedef iqtgui::TDesignerGuiCompBase<Ui::CExtendedConsoleGuiComp> BaseClass;
	
	I_BEGIN_COMPONENT(CExtendedConsoleGuiComp);
		I_ASSIGN(m_viewProviderGuiCompPtr, "ViewProvider", "Provider of the 2D-View", true, "ViewProvider");
		I_ASSIGN(m_toolBarCompPtr, "ToolBar", "Tool bar showing the commands of the view provider", false, "ToolBar");
		I_ASSIGN(m_statusInfoGuiCompPtr, "StatusInfoGui", "UI for status information of the 2D-console", false, "StatusInfoGui");
	I_END_COMPONENT;

protected:
	// reimplemented (CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();

private:
	I_REF(iqtgui::IGuiObject, m_viewProviderGuiCompPtr);
	I_REF(iqtgui::IGuiObject, m_toolBarCompPtr);
	I_REF(iqtgui::IGuiObject, m_statusInfoGuiCompPtr);
};


} // namespace iqt2d


#endif // !iqt2d_CExtendedConsoleGuiComp_included


