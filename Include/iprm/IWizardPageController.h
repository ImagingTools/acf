#ifndef iprm_IWizardPageController_included
#define iprm_IWizardPageController_included


#include "istd/IPolymorphic.h"


namespace imod
{
	class IModel;
}


namespace iprm
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
	virtual bool UpdateWizardPage(iprm::IWizardPageInfo& page, imod::IModel& pageDataModel) = 0;
};


} // namespace iprm


#endif // !iprm_IWizardPageController_included


