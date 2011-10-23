#ifndef iprm_IWizardNavigationController_included
#define iprm_IWizardNavigationController_included


#include "istd/IPolymorphic.h"


namespace iprm
{


/**
	Interface for wizard navigation.
*/
class IWizardNavigationController: virtual public istd::IPolymorphic
{
public:

	/**
		Get the ID of the next wizard page for the current page.
		\param currentPageId	ID of the current wizard page.
	*/
	virtual int GetNextPageId(int currentPageId) = 0;
};


} // namespace iprm


#endif // !iprm_IWizardNavigationController_included


