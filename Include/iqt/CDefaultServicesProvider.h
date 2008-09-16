#ifndef iqt_CDefaultServicesProvider_included
#define iqt_CDefaultServicesProvider_included


#include "iqt/CTimer.h"


namespace iqt
{


/**
	Default services provider for qt based services
 */
class CDefaultServicesProvider
{
public:
	static void RegisterServices();

private:
	static iqt::CTimer s_timer;
};


} // namespace iqt


#endif // !iqt_CDefaultServicesProvider_included


