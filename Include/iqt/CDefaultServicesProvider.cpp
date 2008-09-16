#include "iqt/CDefaultServicesProvider.h"

#include "istd/CStaticServicesProvider.h"


namespace iqt
{


void CDefaultServicesProvider::RegisterServices()
{
	istd::CStaticServicesProvider::RegisterService<isys::ITimer>(&s_timer);
}


iqt::CTimer CDefaultServicesProvider::s_timer;


} // namespace iqt


