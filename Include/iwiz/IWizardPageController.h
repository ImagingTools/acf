#ifndef iwiz_IWizardPageController_included
#define iwiz_IWizardPageController_included


#include "istd/IPolymorphic.h"


namespace imod
{
	class IModel;
}


namespace iwiz
{


class IWizardPageInfo;


/**
	Interface for a wizard page controller.
	Controller will be notified about changes in the wizard page and 
	can set page status and/or navigation flow to the page object via UpdatePage method.
*/
class IWizardPageController: virtual public istd::IPolymorphic
{
public:
	/**
		The method will be called on changes in the data model of the wizard page.
		\param pageDataModel data model of the wizard page
		\param page	wizard page, which has triggered this event
	*/
	virtual bool UpdateWizardPage(iwiz::IWizardPageInfo& page, imod::IModel& pageDataModel) = 0;
};


} // namespace iwiz


#endif // !iwiz_IWizardPageController_included


