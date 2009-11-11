#ifndef ilibav_CLibAvVideoDecoder_included
#define ilibav_CLibAvVideoDecoder_included


// LIBAV includes
extern "C"{
#include "libavformat/avformat.h"
}

#include "icomp/CComponentBase.h"

#include "imm/IVideoController.h"


namespace ilibav
{


class CLibAvVideoDecoder:
			public icomp::CComponentBase,
			virtual public imm::IVideoController
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CLibAvVideoDecoder);
		I_REGISTER_INTERFACE(istd::IChangeable);
		I_REGISTER_INTERFACE(imm::IMediaController);
		I_REGISTER_INTERFACE(imm::IVideoInfo);
		I_REGISTER_INTERFACE(imm::IVideoController);
	I_END_COMPONENT();

	CLibAvVideoDecoder();

	// reimplemented (icomp::IComponent)
	void OnComponentCreated();
	void OnComponentDestroyed();

	// reimplemented (imm::IMediaController)
	virtual istd::CString GetOpenedMediumUrl() const;
	virtual bool OpenMediumUrl(const istd::CString& url, bool autoPlay = true);
	virtual void CloseMedium();
	virtual bool IsPlaying() const;
	virtual bool SetPlaying(bool state = true);
	virtual double GetMediumLength() const;
	virtual double GetCurrentPosition() const;
	virtual bool SetCurrentPosition(double position);
	virtual int	GetSupportedFeatures() const;

	// reimplemented (imm::IVideoInfo)
	virtual int GetFramesCount() const;
	virtual double GetFrameIntervall() const;
	virtual istd::CIndex2d GetFrameSize() const;
	virtual double GetPixelAspectRatio() const;

	// reimplemented (imm::IVideoController)
	virtual int GetCurrentFrame() const;
	virtual bool SetCurrentFrame(int frameIndex);
	virtual bool GrabFrame(iimg::IBitmap& result, int frameIndex = -1) const;

protected:
	bool ReadNextFrame();

private:
	int m_streamId;
	AVFormatContext* m_formatContextPtr;
	AVCodecContext* m_codecContextPtr;
	AVCodec* m_codecPtr;

	AVFrame* m_framePtr;
	AVFrame* m_frameRgbPtr;

	istd::TDelPtr<I_BYTE, true> m_imageBufferPtr;

	AVPacket m_packet;
	int m_bytesRemaining;
	uint8_t* m_rawDataPtr;

	istd::CString m_currentUrl;
};


} // namespace ilibav


#endif // !ilibav_CLibAvVideoDecoder_included

