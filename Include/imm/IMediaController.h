#ifndef imm_IMediaController_included
#define imm_IMediaController_included


#include "istd/IChangeable.h"
#include "istd/CString.h"

#include "imm/imm.h"


namespace imm
{


class IMediaController: virtual public istd::IChangeable
{
public:
	enum ChangeFlags
	{
		CF_STATUS = 0x0400000,
		CF_MEDIA_POSITION = 0x0800000,
	};

	enum SupportedFeatures
	{
		SF_PLAY = 0x1,
		SF_SEEK = 0x2,
		SF_AUTO_PLAY = 0x4
	};

	/**
		Get URL of the medium object.
	*/
	virtual istd::CString GetOpenedMediumUrl() const = 0;

	/**
		Open some media file.
	*/
	virtual bool OpenMediumUrl(const istd::CString& url, bool autoPlay = true) = 0;

	/**
		Close this media player.
		It stops playing and closes media file.
	*/
	virtual void CloseMedium() = 0;

	/**
		Return true, if media is currntly playing.
	*/
	virtual bool IsPlaying() const = 0;

	/**
		Play or pause current media.
	*/
	virtual bool SetPlaying(bool state = true) = 0;

	/**
		Return length of opened media.
	*/
	virtual double GetMediumLength() const = 0;

	/**
		Get current position.
	*/
	virtual double GetCurrentPosition() const = 0;

	/**
		Skip to some position of media stream.
	*/
	virtual bool SetCurrentPosition(double position) = 0;

	/**
		Get info about features, that are supported by this media controller.
		\sa SupportedFeatures
	*/
	virtual int GetSupportedFeatures() const = 0;
};


} // namespace imm


#endif // !imm_IMediaController_included


