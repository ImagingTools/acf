#ifndef WizardPck_included
#define WizardPck_included


#include "icomp/TModelCompWrap.h"


#include "iwiz/CLinearWizardControllerComp.h"
#include "iwiz/CWizardPageComp.h"


/**
	Wizard controller package.
*/
namespace WizardPck
{


typedef iwiz::CLinearWizardControllerComp LinearWizardController;
typedef icomp::TModelCompWrap<
			iwiz::CWizardPageComp> WizardPage;


} // namespace WizardPck


#endif // !WizardPck_included


