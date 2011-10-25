#ifndef WizPck_included
#define WizPck_included


#include "icomp/TModelCompWrap.h"


#include "iwiz/CLinearWizardControllerComp.h"
#include "iwiz/CWizardPageComp.h"


/**
	Wizard controller package.
*/
namespace WizPck
{


typedef iwiz::CLinearWizardControllerComp LinearWizardController;
typedef icomp::TModelCompWrap<
			iwiz::CWizardPageComp> WizardPage;


} // namespace WizPck


#endif // !WizPck_included


