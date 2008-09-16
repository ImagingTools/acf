#ifndef inat_included
#define inat_included


#ifdef _WIN32
#include "iwin/iwin.h"
#elif
#error Operating system not supported
#endif


/**
\defgroup inat Native System
\ingroup ACF
@{
*/

/** 
@} 
*/

/**
	\namespace inat
	\brief This namespace contains implementations for the compile-time native plattform
*/
namespace inat
{


#ifdef _WIN32
	using namespace iwin;
//#elif
#endif


} // namespace inat


#endif // !inat_included
