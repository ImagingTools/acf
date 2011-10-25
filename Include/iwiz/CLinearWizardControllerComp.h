#ifndef iwiz_CLinearWizardControllerComp_included
#define iwiz_CLinearWizardControllerComp_included


#include "icomp/CComponentBase.h"

#include "iprm/IParamsManager.h"

#include "iwiz/IWizardNavigationController.h"
#include "iwiz/IWizardPageController.h"


namespace iwiz
{


/**
	Component implementation of a wizard controller.
*/
class CLinearWizardControllerComp:
			public icomp::CComponentBase,
			virtual public iwiz::IWizardNavigationController
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CLinearWizardControllerComp);
		I_REGISTER_INTERFACE(iwiz::IWizardNavigationController);
		I_ASSIGN(m_wizardPagesCompPtr, "WizardPages", "Container of wizard page data", true, "WizardPages");
	I_END_COMPONENT;

	int GetPagesCount() const;

	// reimplemented (iwiz::IWizardNavigationController)
	virtual int GetNextPageId(int currentPage);

private:
	I_REF(iprm::IParamsManager, m_wizardPagesCompPtr);
};


} // namespace iwiz


#endif // !iwiz_CLinearWizardControllerComp_included


