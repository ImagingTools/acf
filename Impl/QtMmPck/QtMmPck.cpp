#include "icomp/export.h"

#include "iqt/CDefaultServicesProvider.h"

#include "QtMmPck.h"


namespace QtMmPck
{


I_EXPORT_SERVICES_PROVIDER(iqt::CDefaultServicesProvider);

I_EXPORT_PACKAGE("QtMm", "Qt multimedia package", "Qt Media Multimedia");

I_EXPORT_COMPONENT(VideoPlayerGui, "GUI used to create video player with parameters and control bar", "Qt GUI Video Medium Multimedia File Loader Play");
I_EXPORT_COMPONENT(PhononVideoViewGui, "Video widget based on Phonon library", "Phonon Qt Widget GUI Video Media Multimedia Play");
I_EXPORT_COMPONENT(MediaLoader, "Loader for multimedia files", "Qt Video Medium Multimedia File Loader Play");
I_EXPORT_COMPONENT(PlaybackControllerGui, "Simple playback controller", "Play Controller Frame Sequence Qt Video Medium Multimedia File Loader Play");
I_EXPORT_COMPONENT(FrameSeqVideoController, "Video controller playing images from directory on external bitmaps", "Frame Sequence Bitmaps Images Qt Video Medium Multimedia Directory Play");
I_EXPORT_COMPONENT(SvgBitmapLoader, "Loader for vector graphics SVG files rendering it to bitmap", "Bitmap Vector Image File Loader Renderer SVG QT Scalable Graphics");


} // namespace QtMmPck


