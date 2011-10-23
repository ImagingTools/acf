#ifndef iprm_IWizardPageInfo_included
#define iprm_IWizardPageInfo_included


#include "istd/IChangeable.h"


namespace iprm
{


/**
	Interface for a wizard page.
*/
class IWizardPageInfo: virtual public istd::IChangeable
{
public:
	enum ChangeFlags
	{
		CF_PAGE_FINISHED = 0x100000	
	};

	/**
		The page processing was finished, the page data is complete set. 
	*/
	virtual bool IsPageFinished() const = 0;

	/**
		Marks a wizard page as finished.

		\sa IsPageFinished
	*/
	virtual void SetPageFinished(bool isPageFinished = true) = 0;
};


} // namespace iprm


#endif // !iprm_IWizardPageInfo_included


