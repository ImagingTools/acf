#include "iqaxmm/CVlcVideoViewGuiComp.h"


// Qt includes
#include <QUrl>

#include "istd/TChangeNotifier.h"

#include "iqt/CTimer.h"


namespace iqaxmm
{


// public methods

CVlcVideoViewGuiComp::CVlcVideoViewGuiComp()
:	m_vlcInputPtr(NULL),
	m_playlistPtr(NULL)
{
}


// reimplemented (iqtgui::CGuiComponentBase)

void CVlcVideoViewGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();
}


void CVlcVideoViewGuiComp::OnGuiDestroyed()
{
	m_vlcInputPtr = NULL;
	m_playlistPtr = NULL;

	m_currentUrl.clear();

	BaseClass::OnGuiDestroyed();
}


// reimplemented (imm::IMediaController)

istd::CString CVlcVideoViewGuiComp::GetOpenedMediumUrl() const
{
	return m_currentUrl;
}


bool CVlcVideoViewGuiComp::OpenMediumUrl(const istd::CString& url, bool autoPlay)
{
	AXVLC::VLCPlugin2* widgetPtr = GetQtWidget();
	if (widgetPtr != NULL){
		istd::CChangeNotifier notifier(this, CF_STATUS);

		m_vlcInputPtr = widgetPtr->input();

		m_playlistPtr = widgetPtr->playlist();
		if (m_playlistPtr != NULL){
			m_currentUrl = url;

			m_playlistPtr->clear();
			m_playlistPtr->add(iqt::GetQString(m_currentUrl));
			m_playlistPtr->play();

			iqt::CTimer timer;
			timer.WaitTo(2);
			if (!autoPlay){
				m_playlistPtr->togglePause();
			}

			return true;
		}
	}

	return false;
}


void CVlcVideoViewGuiComp::CloseMedium()
{
	if (m_playlistPtr != NULL){
		istd::CChangeNotifier notifier(this, CF_STATUS);

		m_playlistPtr->stop();
		m_playlistPtr->clear();
	}
}


bool CVlcVideoViewGuiComp::IsPlaying() const
{
	if (m_playlistPtr != NULL){
		return m_playlistPtr->isPlaying();
	}

	return false;
}


bool CVlcVideoViewGuiComp::SetPlaying(bool state)
{
	if (state != IsPlaying()){
		istd::CChangeNotifier notifier(this, CF_STATUS);

		if (m_playlistPtr != NULL){
			m_playlistPtr->togglePause();

			return true;
		}
	}

	return false;
}


double CVlcVideoViewGuiComp::GetMediumLength() const
{
	if (m_vlcInputPtr != NULL){
		return m_vlcInputPtr->Length() * 0.001;
	}

	return 0;
}


double CVlcVideoViewGuiComp::GetCurrentPosition() const
{
	if (m_vlcInputPtr != NULL){
		return m_vlcInputPtr->Time() * 0.001;
	}

	return 0;
}


bool CVlcVideoViewGuiComp::SetCurrentPosition(double position)
{
	if (m_vlcInputPtr != NULL){
		m_vlcInputPtr->SetTime(int(position * 1000));

		return true;
	}

	return false;
}


// reimplemented (imm::IVideoInfo)

int CVlcVideoViewGuiComp::GetFramesCount() const
{
	return int(GetMediumLength() * *m_framesPerSecondAttrPtr);
}


double CVlcVideoViewGuiComp::GetFrameTimeDiff() const
{
	return 1.0 / *m_framesPerSecondAttrPtr;
}


istd::CIndex2d CVlcVideoViewGuiComp::GetFrameSize() const
{
	return istd::CIndex2d::GetInvalid();
}


double CVlcVideoViewGuiComp::GetPixelAspectRatio() const
{
	return 1;
}


// reimplemented (imm::IVideoController)

int CVlcVideoViewGuiComp::GetCurrentFrame() const
{
	return int(GetCurrentPosition() * *m_framesPerSecondAttrPtr + 0.5);
}


bool CVlcVideoViewGuiComp::SetCurrentFrame(int frameIndex)
{
	return SetCurrentPosition(frameIndex / *m_framesPerSecondAttrPtr);
}


bool CVlcVideoViewGuiComp::GrabFrame(iimg::IBitmap& /*result*/, int /*frameIndex*/) const
{
	return false;
}


} // namespace iqaxmm


