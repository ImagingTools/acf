#include "iqt/CDefaultServicesProvider.h"


// ACF includes
#include "istd/TSingleFactory.h"
#include "istd/CStaticServicesProvider.h"

#include "iqt/CProcessEnvironment.h"
#include "iqt/CCriticalSection.h"
#include "iqt/CFileSystem.h"
#include "iqt/CDefaultRegistryLoaderProvider.h"
#include "iqt/CSystemEnvironment.h"
#include "iqt/CLocalizer.h"
#include "iqt/CTrace.h"


namespace iqt
{


void CDefaultServicesProvider::RegisterServices()
{
	static iqt::CTrace trace;
	istd::CStaticServicesProvider::RegisterService<istd::ITrace>(&trace);

	static iqt::CProcessEnvironment applicationEnvironment;
	istd::CStaticServicesProvider::RegisterService<isys::IProcessEnvironment>(&applicationEnvironment);

	static iqt::CCriticalSection criticalSection;
	istd::CStaticServicesProvider::RegisterService<isys::ICriticalSection>(&criticalSection);

	static iqt::CFileSystem fileSystem;
	istd::CStaticServicesProvider::RegisterService<isys::IFileSystem>(&fileSystem);

	static iqt::CSystemEnvironment systemEnvironment;
	istd::CStaticServicesProvider::RegisterService<isys::ISystemEnvironment>(&systemEnvironment);

	static iqt::CLocalizer localizer;
	istd::CStaticServicesProvider::RegisterService<istd::ILocalizer>(&localizer);

	static istd::TSingleFactory<isys::ICriticalSection, iqt::CCriticalSection> criticalSectionFactory("");
	istd::CStaticServicesProvider::RegisterFactory(&criticalSectionFactory);
}


} // namespace iqt


