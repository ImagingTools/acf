#ifndef iwin_CDefaultServicesProvider_included
#define iwin_CDefaultServicesProvider_included


#include "iwin/CTimer.h"
#include "iwin/CApplicationEnvironment.h"
#include "iwin/CCriticalSection.h"
#include "iwin/CSystemEnvironment.h"


namespace iwin
{


/**
	Default services provider for winfows plattform
 */
class CDefaultServicesProvider
{
public:
	static void RegisterServices();

private:
	static iwin::CTimer s_timer;
	static iwin::CApplicationEnvironment s_applicationEnvironment;
	static iwin::CSystemEnvironment s_systemEnvironment;
	static iwin::CCriticalSection s_criticalSection;
};


} // namespace iwin


#endif // !iwin_CDefaultServicesProvider_included


