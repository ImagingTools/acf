#ifndef iqtgui_CVersionInfoGuiComp_included
#define iqtgui_CVersionInfoGuiComp_included


// ACF includes
#include "iqtgui/TDesignerGuiCompBase.h"
#include "iser/IVersionInfo.h"

#include "Generated/ui_CVersionInfoGuiComp.h"


namespace iqtgui
{


/**
	GUI component for showing the list of module versions.
*/
class CVersionInfoGuiComp: public iqtgui::TDesignerGuiCompBase<Ui::CVersionInfoGuiComp>
{
public:
	typedef iqtgui::TDesignerGuiCompBase<Ui::CVersionInfoGuiComp> BaseClass;

	I_BEGIN_COMPONENT(CVersionInfoGuiComp);
		I_ASSIGN_MULTI_0(m_versionInfosCompPtr, "VersionInfos", "List of version infos", true);
	I_END_COMPONENT;

protected:
	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();

private:
	I_MULTIREF(iser::IVersionInfo, m_versionInfosCompPtr);
};


} // namespace iqtgui


#endif // !iqtgui_CVersionInfoGuiComp_included


