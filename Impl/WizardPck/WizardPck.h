#ifndef WizardPck_included
#define WizardPck_included


#include "icomp/TModelCompWrap.h"


#include "iwiz/CWizardDocumentComp.h"
#include "iwiz/CWizardPageComp.h"


/**
	Wizard controller package.
*/
namespace WizardPck
{


typedef icomp::TModelCompWrap<iwiz::CWizardDocumentComp> WizardDocument;
typedef icomp::TModelCompWrap<iwiz::CWizardPageComp> WizardPage;


} // namespace WizardPck


#endif // !WizardPck_included


