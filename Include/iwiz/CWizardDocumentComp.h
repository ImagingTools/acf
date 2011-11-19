#ifndef iwiz_CWizardDocumentComp_included
#define iwiz_CWizardDocumentComp_included


#include "icomp/CComponentBase.h"

#include "iprm/CParamsManagerComp.h"

#include "iwiz/IWizardNavigationController.h"
#include "iwiz/IParamsManagerWizard.h"


namespace iwiz
{


/**
	Component implementation of a wizard controller.
*/
class CWizardDocumentComp:
			public iprm::CParamsManagerComp,
			virtual public IParamsManagerWizard
{
public:
	typedef iprm::CParamsManagerComp BaseClass;

	I_BEGIN_COMPONENT(CWizardDocumentComp);
		I_REGISTER_INTERFACE(IParamsManagerWizard);
		I_REGISTER_INTERFACE(IWizardNavigationController);
	I_END_COMPONENT;

	// reimplemented (iwiz::IParamsManagerWizard)
	virtual bool IsPageSelectionAllowed(int pageIndex) const;

	// reimplemented (iwiz::IWizardNavigationController)
	virtual bool IsFinishAllowed() const;
	virtual int GetPrevPageIndex() const;
	virtual int GetNextPageIndex() const;
};


} // namespace iwiz


#endif // !iwiz_CWizardDocumentComp_included


