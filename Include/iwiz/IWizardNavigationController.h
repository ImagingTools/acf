#ifndef iwiz_IWizardNavigationController_included
#define iwiz_IWizardNavigationController_included


#include "istd/IPolymorphic.h"


namespace iwiz
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


} // namespace iwiz


#endif // !iwiz_IWizardNavigationController_included


