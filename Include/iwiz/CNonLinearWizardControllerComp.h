#ifndef iwiz_CNonLinearWizardControllerComp_included
#define iwiz_CNonLinearWizardControllerComp_included


#include "icomp/CComponentBase.h"


#include "iprm/IParamsManager.h"

#include "iwiz/IWizardNavigationController.h"
#include "iwiz/IWizardPageController.h"


namespace iwiz
{


/**
	Component implementation of a wizard controller.
*/
class CNonLinearWizardControllerComp:
			public icomp::CComponentBase,
			virtual public iwiz::IWizardNavigationController,
			virtual public iwiz::IWizardPageController
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CNonLinearWizardControllerComp);
		I_REGISTER_INTERFACE(iwiz::IWizardNavigationController);
		I_REGISTER_INTERFACE(iwiz::IWizardPageController);
		I_ASSIGN(m_wizardPagesCompPtr, "WizardPages", "Container of wizard page data", true, "WizardPages");
	I_END_COMPONENT;

	int GetPagesCount() const;

	// reimplemented (iwiz::IWizardNavigationController)
	virtual int GetNextPageId(int currentPage);

	// reimplemented (iwiz::IWizardPageController)
	virtual bool UpdateWizardPage(iwiz::IWizardPageInfo& page, imod::IModel& pageDataModel);

private:
	I_REF(iprm::IParamsManager, m_wizardPagesCompPtr);
};


} // namespace iwiz


#endif // !iwiz_CNonLinearWizardControllerComp_included


