#include "icomp/export.h"

#include "iqt/CDefaultServicesProvider.h"

#include "AbcPck.h"


static struct StaticInit
{
	StaticInit()
	{
		Q_INIT_RESOURCE(iqtgui);
	}

} instance;


namespace AbcPck
{


I_EXPORT_SERVICES_PROVIDER(iqt::CDefaultServicesProvider);

I_EXPORT_PACKAGE("Abc", "Application Business Components", "Qt Application Business Enterprise");

I_EXPORT_COMPONENT(FolderMonitor, "This component observes changes in a given file folder", "Qt File Dir Filter Observer");
I_EXPORT_COMPONENT(ServiceApplication, "An application as service/daemon", "Qt Application Service Daemon");


} // namespace AbcPck


