#include "icomp/export.h"

#include "iqt/CDefaultServicesProvider.h"

#include "QtWizPck.h"


static struct StaticInit
{
	StaticInit()
	{
		Q_INIT_RESOURCE(iqtgui);
	}

} instance;


namespace QtWizPck
{


I_EXPORT_SERVICES_PROVIDER(iqt::CDefaultServicesProvider);

I_EXPORT_PACKAGE(
			"Acf/Qt/Wizard",
			"Wizard UI package",
			IM_PROJECT("ACF") IM_TAG("Qt GUI Wizard") IM_COMPANY("ImagingTools") IM_AUTHOR("\"Witold Gantzke\" \"Kirill Lepskiy\""));


I_EXPORT_COMPONENT(
			WizardGui,
			"Wizard-based parameter editor",
			"Wizard");

I_EXPORT_COMPONENT(
			WizardOptionsListGui,
			"GUI for the list of wizard options",
			"Wizard");


} // namespace QtWizPck


