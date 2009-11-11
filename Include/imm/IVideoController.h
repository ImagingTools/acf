#ifndef imm_IVideoController_included
#define imm_IVideoController_included


#include "iimg/IBitmap.h"

#include "imm/IMediaController.h"
#include "imm/IVideoInfo.h"


namespace imm
{


class IVideoController: virtual public IMediaController, virtual public IVideoInfo
{
public:
	enum SupportedFeatures{
		SF_GRAB_CURRENT = 0x100,
		SF_GRAB_ANY = 0x200
	};

	/**
		Get index of current frame.
	*/
	virtual int GetCurrentFrame() const = 0;

	/**
		Set index of current frame.
		This works similiar to imm::IMediaController::SetCurrentPosition, but the parameter is frame index.
	*/
	virtual bool SetCurrentFrame(int frameIndex) = 0;

	/**
		Grab some frame as bitmap.
		\param	result		result bitmap object.
		\param	frameIndex	index of frame or negative value if current frame should be grabbed.
		\return	true if it was possible to grab the frame.
	*/
	virtual bool GrabFrame(iimg::IBitmap& result, int frameIndex = -1) const = 0;
};


} // namespace imm


#endif // !imm_IVideoController_included


