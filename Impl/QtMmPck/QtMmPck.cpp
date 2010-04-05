#include "icomp/export.h"

#include "iqt/CDefaultServicesProvider.h"

#include "QtMmPck.h"


namespace QtMmPck
{


using namespace icomp;


I_EXPORT_SERVICES_PROVIDER(iqt::CDefaultServicesProvider);

I_EXPORT_PACKAGE(
			"QtMm",
			"Qt multimedia package",
			"Media" IM_PROJECT("ACF") IM_TAG("Qt Multimedia") IM_COMPANY("ImagingTools") IM_AUTHOR("\"Witold Gantzke\" \"Kirill Lepskiy\""));

I_EXPORT_COMPONENT(
			VideoPlayerGui,
			"GUI used to create video player with parameters and control bar",
			"Qt Video File Loader Play" IM_TAG("GUI"));

I_EXPORT_COMPONENT(
			PhononVideoViewGui,
			"Video widget based on Phonon library",
			"Phonon Widget Video Play" IM_TAG("GUI"));

I_EXPORT_COMPONENT(
			MediaLoader,
			"Loader for multimedia files",
			"Video File Loader Play" IM_TAG("Loader"));

I_EXPORT_COMPONENT(
			PlaybackControllerGui,
			"Simple playback controller",
			"Play Controller Frame Sequence Video File Loader Play" IM_TAG("GUI"));

I_EXPORT_COMPONENT(
			FrameSeqVideoController,
			"Video controller playing images from directory on external bitmaps",
			"Frame Sequence Bitmaps Images Video Medium Multimedia Directory Play" IM_TAG("Image"));

I_EXPORT_COMPONENT(
			SvgBitmapLoader,
			"Loader for vector graphics SVG files rendering it to bitmap",
			"Bitmap Vector Renderer SVG QT Scalable Graphics" IM_TAG("Image File Loader"));


} // namespace QtMmPck


