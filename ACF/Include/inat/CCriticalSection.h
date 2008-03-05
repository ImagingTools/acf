#ifndef inat_CCriticalSection_included
#define inat_CCriticalSection_included


#include "inat/inat.h"


#ifdef _WIN32
#include "iwin/CCriticalSection.h"
#else
#ifdef QT_CORE_LIB
#include "iqt/CCriticalSection.h"
#endif
#elif
#error Operating system not supported
#endif


#endif // !inat_CCriticalSection_included


