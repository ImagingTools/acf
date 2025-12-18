#ifndef iqtgui_CCalendarGuiComp_included
#define iqtgui_CCalendarGuiComp_included


// ACF includes
#include <iqtgui/TDesignerGuiCompBase.h>

#include <GeneratedFiles/iqtgui/ui_CCalendarGuiComp.h>


namespace iqtgui
{


/**
	Component for displaying a calendar.
*/
class CCalendarGuiComp: public iqtgui::TDesignerGuiCompBase<Ui::CCalendarGuiComp>
{
	Q_OBJECT
public:
	typedef iqtgui::TDesignerGuiCompBase<Ui::CCalendarGuiComp> BaseClass;

	I_BEGIN_COMPONENT(CCalendarGuiComp);
	I_END_COMPONENT;

protected:
	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated() override;
	virtual void OnGuiRetranslate() override;
};


} // namespace iqtgui


#endif // !iqtgui_CCalendarGuiComp_included


