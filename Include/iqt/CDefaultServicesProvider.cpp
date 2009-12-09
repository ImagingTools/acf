#include "iqt/CDefaultServicesProvider.h"


#include "istd/TSingleFactory.h"
#include "istd/CStaticServicesProvider.h"

#include "iqt/CTimer.h"
#include "iqt/CApplicationEnvironment.h"
#include "iqt/CCriticalSection.h"
#include "iqt/CFileSystem.h"
#include "iqt/CDefaultRegistryLoaderProvider.h"
#include "iqt/CSystemEnvironment.h"


namespace iqt
{


void CDefaultServicesProvider::RegisterServices()
{
	static iqt::CApplicationEnvironment applicationEnvironment;
	istd::CStaticServicesProvider::RegisterService<isys::IApplicationEnvironment>(&applicationEnvironment);

	static iqt::CCriticalSection criticalSection;
	istd::CStaticServicesProvider::RegisterService<isys::ICriticalSection>(&criticalSection);

	static iqt::CFileSystem fileSystem;
	istd::CStaticServicesProvider::RegisterService<isys::IFileSystem>(&fileSystem);

	static iqt::CDefaultRegistryLoaderProvider registryLoaderProvider;
	istd::CStaticServicesProvider::RegisterService<icomp::IRegistryLoaderProvider>(&registryLoaderProvider);

	static iqt::CSystemEnvironment systemEnvironment;
	istd::CStaticServicesProvider::RegisterService<isys::ISystemEnvironment>(&systemEnvironment);

	static istd::TSingleFactory<isys::ITimer, iqt::CTimer> timerFactory("");
	istd::CStaticServicesProvider::RegisterFactory(&timerFactory);
}


} // namespace iqt


