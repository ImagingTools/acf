#ifndef inat_CCriticalSection_included
#define inat_CCriticalSection_included


#include "inat/inat.h"


#ifdef _WIN32
#include "iwin/CCriticalSection.h"
#elif
#error Operating system not supported
#endif


#endif // !inat_CCriticalSection_included


