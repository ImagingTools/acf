#ifndef inat_CDllFunctionsProvider_included
#define inat_CDllFunctionsProvider_included


#include "inat/inat.h"


#ifdef _WIN32
#include "iwin/CDllFunctionsProvider.h"
#elif defined QT_CORE_LIB
#include "iqt/CDllFunctionsProvider.h"
#elif
#error Operating system not supported
#endif


#endif // !inat_CDllFunctionsProvider_included


