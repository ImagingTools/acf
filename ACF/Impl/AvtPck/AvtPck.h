#ifndef AvtPck_included
#define AvtPck_included


#include "icomp/TModelCompWrap.h"
#include "icomp/TMakeComponentWrap.h"

#include "iavt/CFireGrabAcquisitionComp.h"
#include "iavt/CAvtTriggerParamsComp.h"



/**
	AVT-specific package.
*/
namespace AvtPck
{


typedef iavt::CFireGrabAcquisitionComp FireGrabAcquisition;
typedef icomp::TModelCompWrap<iavt::CAvtTriggerParamsComp> AvtTriggerParams;


} // namespace AvtPck


#endif // !AvtPck_included


