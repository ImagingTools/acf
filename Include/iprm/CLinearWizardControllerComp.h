#ifndef iprm_CLinearWizardControllerComp_included
#define iprm_CLinearWizardControllerComp_included


#include "iprm/IParamsManager.h"
#include "iprm/IWizardNavigationController.h"
#include "iprm/IWizardPageController.h"

#include "icomp/CComponentBase.h"


namespace iprm
{


/**
	Component implementation of a wizard controller.
*/
class CLinearWizardControllerComp:
			public icomp::CComponentBase,
			virtual public iprm::IWizardNavigationController
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CLinearWizardControllerComp);
		I_REGISTER_INTERFACE(iprm::IWizardNavigationController);
		I_ASSIGN(m_wizardPagesCompPtr, "WizardPages", "Container of wizard page data", true, "WizardPages");
	I_END_COMPONENT;

	int GetPagesCount() const;

	// reimplemented (iprm::IWizardNavigationController)
	virtual int GetNextPageId(int currentPage);

private:
	I_REF(iprm::IParamsManager, m_wizardPagesCompPtr);
};


} // namespace iprm


#endif // !iprm_CLinearWizardControllerComp_included


