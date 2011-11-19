#ifndef iwiz_IParamsManagerWizard_included
#define iwiz_IParamsManagerWizard_included


#include "iprm/IParamsManager.h"

#include "iwiz/IWizardNavigationController.h"


namespace iwiz
{


/**
	Interface for wizard implementation based on parameter manager.
	In this concept each wizard page is stored as \c iprm::IParamsSet object.
	Pages are accessible using standard iprm::IParamsManager interface.
	Typically no inserting or deleting of page is supported.
*/
class IParamsManagerWizard:
			virtual public IWizardNavigationController,
			virtual public iprm::IParamsManager
{
public:
	/**
		Check, if selection of some wizard page is allowed.
		It is used to avoid trying switch to inactive pages.
		\param	pageIndex	index of page.
							It has the same interpretation as e.g. \c index in iprm::IParamsManager::GetParamsSet or
							\c index in iprm::ISelectionConstraints::GetOptionName.
	*/
	virtual bool IsPageSelectionAllowed(int pageIndex) const = 0;
};


} // namespace iwiz


#endif // !iwiz_IParamsManagerWizard_included


