#include "icomp/export.h"

#include "iqt/CDefaultServicesProvider.h"

#include "QtMmPck.h"


namespace QtMmPck
{


I_EXPORT_SERVICES_PROVIDER(iqt::CDefaultServicesProvider);

I_EXPORT_PACKAGE("QtMm", "Qt multimedia package", "Qt Media Multimedia");

I_EXPORT_COMPONENT(PhononVideoView, "Video widget based on Phonon library", "Phonon Qt Widget GUI Video Media Multimedia Play");
I_EXPORT_COMPONENT(MediaLoader, "Loader for multimedia files", "Qt Video Medium Multimedia File Loader Play");


} // namespace QtMmPck


