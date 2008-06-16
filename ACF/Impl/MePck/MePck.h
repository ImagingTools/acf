#ifndef MePck_included
#define MePck_included


#include "icomp/TModelCompWrap.h"
#include "icomp/TMakeComponentWrap.h"

#include "CMeilhausSimpleComp.h"
#include "CChannelAddressParamComp.h"



/**
	Meilhaus-specific package.
*/
namespace MePck
{


typedef imebase::CMeilhausSimpleComp MeilhausSimpleAcquisition;
typedef icomp::TModelCompWrap<imebase::CChannelAddressParamComp> ChannelAddressParam;


} // namespace MePck


#endif // !MePck_included


