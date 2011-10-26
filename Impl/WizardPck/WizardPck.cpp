#include "WizardPck.h"


#include "icomp/export.h"


namespace WizardPck
{


I_EXPORT_DEFAULT_SERVICES;

I_EXPORT_PACKAGE(
			"Acf/Wizard",
			"Wizard data model and controller package",
			IM_PROJECT("ACF") IM_TAG("Wizard") IM_COMPANY("ImagingTools") IM_AUTHOR("\"Witold Gantzke\" \"Kirill Lepskiy\""));

I_EXPORT_COMPONENT(
			LinearWizardController,
			"Wizard controller",
			"Wizard Parameter" IM_CATEGORY(I_DATA_MODEL));

I_EXPORT_COMPONENT(
			WizardPage,
			"Single wizard page data",
			"Wizard Parameter" IM_CATEGORY(I_DATA_MODEL));


} // namespace WizardPck


