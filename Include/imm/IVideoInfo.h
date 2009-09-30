#ifndef imm_IVideoInfo_included
#define imm_IVideoInfo_included


#include "istd/IPolymorphic.h"
#include "istd/CIndex2d.h"

#include "imm/imm.h"


namespace imm
{


class IVideoInfo: virtual public istd::IPolymorphic
{
public:
	/**
		Return length of video in seconds.
	*/
	virtual int GetFramesCount() const = 0;

	/**
		Return time difference between frames.
	*/
	virtual double GetFrameIntervall() const = 0;

	/**
		Return size in pixels of single frame.
		If size is unknown, it returns invalid index object. \sa istd::CIndex2d::IsValid().
	*/
	virtual istd::CIndex2d GetFrameSize() const = 0;

	/**
		Get frame aspect ratio.
		Pixel aspect ratio is proportion of pixel with to pixel height.
	*/
	virtual double GetPixelAspectRatio() const = 0;
};


} // namespace imm


#endif // !imm_IVideoInfo_included


