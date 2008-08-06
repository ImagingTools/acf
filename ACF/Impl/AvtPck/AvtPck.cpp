#include "icomp/export.h"

#include "AvtPck.h"


namespace AvtPck
{

I_EXPORT_DEFAULT_SERVICES

I_EXPORT_PACKAGE("AvtPck", "Standard Applied Vision Technologies (AVT) hardware package based on Qt", "AVT Applied Vision Technologies Qt Standard");

I_EXPORT_COMPONENT(FireGrabAcquisition, "Implementation of camera interface using FireGrab API", "AVT Grabber Camera Acquisition Bitmap Image FireGrab");
I_EXPORT_COMPONENT(AvtTriggerParams, "Trigger params for AVT cameras", "AVT Grabber Camera Acquisition Bitmap Image FireGrab Parameter");


} // namespace AvtPck


