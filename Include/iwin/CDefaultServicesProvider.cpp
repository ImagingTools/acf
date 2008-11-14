#include "iwin/CDefaultServicesProvider.h"

#include "istd/CStaticServicesProvider.h"


namespace iwin
{


void CDefaultServicesProvider::RegisterServices()
{
	istd::CStaticServicesProvider::RegisterService<isys::ITimer>(&s_timer);
	istd::CStaticServicesProvider::RegisterService<isys::IApplicationEnvironment>(&s_applicationEnvironment);
	istd::CStaticServicesProvider::RegisterService<isys::ICriticalSection>(&s_criticalSection);
	istd::CStaticServicesProvider::RegisterService<isys::ISystemEnvironment>(&s_systemEnvironment);
}


iwin::CTimer CDefaultServicesProvider::s_timer;
iwin::CApplicationEnvironment CDefaultServicesProvider::s_applicationEnvironment;
iwin::CCriticalSection CDefaultServicesProvider::s_criticalSection;
iwin::CSystemEnvironment CDefaultServicesProvider::s_systemEnvironment;


} // namespace iwin


