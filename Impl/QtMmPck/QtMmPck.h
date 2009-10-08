#ifndef QtMmPck_included
#define QtMmPck_included


#include "icomp/TModelCompWrap.h"

#include "iqtmm/CVideoPlayerGuiComp.h"
#include "iqtmm/CPhononVideoViewGuiComp.h"
#include "iqtmm/CMediaLoaderComp.h"
#include "iqtmm/CPlaybackControllerGuiComp.h"
#include "iqtmm/CFrameSeqVideoControllerComp.h"
#include "iqtmm/CSvgBitmapLoaderComp.h"


/**
	Qt based multimedia package.
*/
namespace QtMmPck
{


typedef iqtmm::CVideoPlayerGuiComp VideoPlayerGui;
typedef iqtmm::CPhononVideoViewGuiComp PhononVideoViewGui;
typedef iqtmm::CMediaLoaderComp MediaLoader;
typedef iqtmm::CPlaybackControllerGuiComp PlaybackControllerGui;
typedef icomp::TModelCompWrap<iqtmm::CFrameSeqVideoControllerComp> FrameSeqVideoController;
typedef iqtmm::CSvgBitmapLoaderComp SvgBitmapLoader;


} // namespace QtMmPck


#endif // !QtMmPck_included


