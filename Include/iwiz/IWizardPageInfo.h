#ifndef iwiz_IWizardPageInfo_included
#define iwiz_IWizardPageInfo_included


#include "istd/IChangeable.h"


namespace iwiz
{


/**
	Interface for a wizard page status info.
*/
class IWizardPageInfo: virtual public istd::IChangeable
{
public:
	enum ChangeFlags
	{
		CF_PAGE_FINISHED = 0x100000	
	};

	enum PageStatus
	{
		PS_FINISHED = 1,
		PS_VISITED = PS_FINISHED << 1
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


} // namespace iwiz


#endif // !iwiz_IWizardPageInfo_included


