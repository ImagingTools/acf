#include "icomp/export.h"

#include "iqt/CDefaultServicesProvider.h"

#include "QaxPck.h"


namespace QaxPck
{


I_EXPORT_SERVICES_PROVIDER(iqt::CDefaultServicesProvider);

I_EXPORT_PACKAGE("Qax", "QActiveX based package", "Qt ActiveX QActiveX COM OLE Windows");

I_EXPORT_COMPONENT(VlcVideoView, "Video widget based on VLC ActiveX", "VLC Qt Widget GUI Video Media Multimedia Play");


} // namespace QaxPck


