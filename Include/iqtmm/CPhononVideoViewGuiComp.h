#ifndef iqtmm_CPhononVideoViewGuiComp_included
#define iqtmm_CPhononVideoViewGuiComp_included


// Qt/Phonon includes
#include <Phonon/VideoWidget>
#include <Phonon/MediaObject>

#include "imm/IVideoController.h"

#include "iqtgui/TGuiComponentBase.h"


namespace iqtmm
{


class CPhononVideoViewGuiComp:
			public iqtgui::TGuiComponentBase<Phonon::VideoWidget>,
			virtual public imm::IVideoController
{
	Q_OBJECT
public:
	typedef iqtgui::TGuiComponentBase<Phonon::VideoWidget> BaseClass;

	I_BEGIN_COMPONENT(CPhononVideoViewGuiComp);
		I_REGISTER_INTERFACE(istd::IChangeable);
		I_REGISTER_INTERFACE(imm::IMediaController);
		I_REGISTER_INTERFACE(imm::IVideoInfo);
		I_REGISTER_INTERFACE(imm::IVideoController);
		I_ASSIGN(m_framesPerSecondAttrPtr, "FramesPerSecond", "Default number of frames per second if this info is unavailable from video", true, 25.0);
	I_END_COMPONENT();

	CPhononVideoViewGuiComp();

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();

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
	void EnsureSync() const;

private:
	Phonon::MediaObject m_mediaObject;

	double m_currentPosition;

	I_ATTR(double, m_framesPerSecondAttrPtr);
};


} // namespace iqtmm


#endif // !iqtmm_CPhononVideoViewGuiComp_included

