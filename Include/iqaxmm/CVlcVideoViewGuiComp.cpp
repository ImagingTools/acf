#include "iqaxmm/CVlcVideoViewGuiComp.h"


// Qt includes
#include <QUrl>
#include <QFrame>
#include <QVBoxLayout>

#include "istd/TChangeNotifier.h"

#include "iqt/CTimer.h"


namespace iqaxmm
{


// public methods

CVlcVideoViewGuiComp::CVlcVideoViewGuiComp()
:	m_vlcWidgetPtr(NULL),
	m_vlcInputPtr(NULL),
	m_playlistPtr(NULL)
{
}


// reimplemented (iqtgui::CGuiComponentBase)

void CVlcVideoViewGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	QFrame* widgetPtr = GetQtWidget();
	if (widgetPtr != NULL){
		widgetPtr->setFrameStyle(QFrame::NoFrame);
		QVBoxLayout* layoutPtr = new QVBoxLayout(widgetPtr);
		layoutPtr->setContentsMargins(0, 0, 0, 0);
		widgetPtr->setLayout(layoutPtr);
	}
}


void CVlcVideoViewGuiComp::OnGuiDestroyed()
{
	if (m_vlcWidgetPtr != NULL){
		delete m_vlcWidgetPtr;
		m_vlcWidgetPtr = NULL;
		m_vlcInputPtr = NULL;
		m_playlistPtr = NULL;
	}

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
	if (m_vlcWidgetPtr != NULL){
		delete m_vlcWidgetPtr;
		m_vlcWidgetPtr = NULL;
		m_vlcInputPtr = NULL;
		m_playlistPtr = NULL;
	}

	QWidget* widgetPtr = GetQtWidget();
	if (widgetPtr != NULL){
		m_vlcWidgetPtr = new AXVLC::VLCPlugin2(widgetPtr);

		QLayout* layoutPtr = widgetPtr->layout();
		if (layoutPtr != NULL){
			layoutPtr->addWidget(m_vlcWidgetPtr);
		}

		istd::CChangeNotifier notifier(this, CF_STATUS);

		m_vlcInputPtr = m_vlcWidgetPtr->input();

		m_playlistPtr = m_vlcWidgetPtr->playlist();
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


int	CVlcVideoViewGuiComp::GetSupportedFeatures() const
{
	return SF_PLAY | SF_AUTO_PLAY | SF_SEEK;
}


// reimplemented (imm::IVideoInfo)

int CVlcVideoViewGuiComp::GetFramesCount() const
{
	return int(GetMediumLength() * *m_framesPerSecondAttrPtr);
}


double CVlcVideoViewGuiComp::GetFrameIntervall() const
{
	return 1.0 / *m_framesPerSecondAttrPtr;
}


istd::CIndex2d CVlcVideoViewGuiComp::GetFrameSize() const
{
	return istd::CIndex2d::GetInvalid();
}


double CVlcVideoViewGuiComp::GetPixelAspectRatio() const
{
	return -1;
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


