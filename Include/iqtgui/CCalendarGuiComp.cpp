#include <iqtgui/CCalendarGuiComp.h>


namespace iqtgui
{


// protected methods

// reimplemented (iqtgui::CGuiComponentBase)

void CCalendarGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	CalendarWidget->setCurrentPage(QDate::currentDate().year(), QDate::currentDate().month());
	CalendarWidget->setSelectedDate(QDate::currentDate());
}


void CCalendarGuiComp::OnGuiRetranslate()
{
	BaseClass::OnGuiRetranslate();
}


} // namespace iqtgui


