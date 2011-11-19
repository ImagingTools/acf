#ifndef iwiz_IWizardNavigationController_included
#define iwiz_IWizardNavigationController_included


#include "istd/IChangeable.h"


namespace iwiz
{


/**
	Interface for wizard navigation.
*/
class IWizardNavigationController: virtual public istd::IChangeable
{
public:
	/**
		Check, if finish is allowed for this wizard.
	*/
	virtual bool IsFinishAllowed() const = 0;
	/**
		Get the ID of the previous wizard page.
	*/
	virtual int GetPrevPageIndex() const = 0;
	/**
		Get the ID of the next wizard page.
	*/
	virtual int GetNextPageIndex() const = 0;
};


} // namespace iwiz


#endif // !iwiz_IWizardNavigationController_included


