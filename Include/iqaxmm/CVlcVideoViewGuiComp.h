#ifndef iqaxmm_CVlcVideoViewGuiComp_included
#define iqaxmm_CVlcVideoViewGuiComp_included


// Qt includes
#include <QFrame>
#include <QAxWidget>

#include "imm/IVideoController.h"

#include "iqtgui/TGuiComponentBase.h"

#include "iqaxmm/Wrapped/axvlc.h"


namespace iqaxmm
{


class CVlcVideoViewGuiComp:
			public iqtgui::TGuiComponentBase<QFrame>,
			virtual public imm::IVideoController
{
public:
	typedef iqtgui::TGuiComponentBase<QFrame> BaseClass;

	I_BEGIN_COMPONENT(CVlcVideoViewGuiComp);
		I_REGISTER_INTERFACE(istd::IChangeable);
		I_REGISTER_INTERFACE(imm::IMediaController);
		I_REGISTER_INTERFACE(imm::IVideoInfo);
		I_REGISTER_INTERFACE(imm::IVideoController);
		I_ASSIGN(m_framesPerSecondAttrPtr, "FramesPerSecond", "Default number of frames per second if this info is unavailable from video", true, 25.0);
	I_END_COMPONENT();

	CVlcVideoViewGuiComp();

	// reimplemented (iqtgui::CGuiComponentBase)
	void OnGuiCreated();
	void OnGuiDestroyed();

	// reimplemented (imm::IMediaController)
	virtual istd::CString GetOpenedMediumUrl() const;
	virtual bool OpenMediumUrl(const istd::CString& url, bool autoPlay = true);
	virtual void CloseMedium();
	virtual bool IsPlaying() const;
	virtual bool SetPlaying(bool state = true);
	virtual double GetMediumLength() const;
	virtual double GetCurrentPosition() const;
	virtual bool SetCurrentPosition(double position);

	// reimplemented (imm::IVideoInfo)
	virtual int GetFramesCount() const;
	virtual double GetFrameTimeDiff() const;
	virtual istd::CIndex2d GetFrameSize() const;
	virtual double GetPixelAspectRatio() const;

	// reimplemented (imm::IVideoController)
	virtual int GetCurrentFrame() const;
	virtual bool SetCurrentFrame(int frameIndex);
	virtual bool GrabFrame(iimg::IBitmap& result, int frameIndex = -1) const;

private:
	AXVLC::VLCPlugin2* m_vlcWidgetPtr;
	AXVLC::IVLCInput* m_vlcInputPtr;
	AXVLC::IVLCPlaylist* m_playlistPtr;

	istd::CString m_currentUrl;

	I_ATTR(double, m_framesPerSecondAttrPtr);
};


} // namespace iqaxmm


#endif // !iqaxmm_CVlcVideoViewGuiComp_included

