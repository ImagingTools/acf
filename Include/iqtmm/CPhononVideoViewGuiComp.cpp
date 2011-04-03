#include "iqtmm/CPhononVideoViewGuiComp.h"


// Qt includes
#include <QCoreApplication>
#include <QEventLoop>
#include <QUrl>


// ACF includes
#include <istd/TChangeNotifier.h>


namespace iqtmm
{


// public methods

CPhononVideoViewGuiComp::CPhononVideoViewGuiComp()
:	m_currentPosition(0)
{
}


// reimplemented (imm::IMediaController)

istd::CString CPhononVideoViewGuiComp::GetOpenedMediumUrl() const
{
	EnsureSync();

	return iqt::GetCString(m_mediaObject.currentSource().url().path());
}


bool CPhononVideoViewGuiComp::OpenMediumUrl(const istd::CString& url, bool autoPlay)
{
	istd::CChangeNotifier notifier(this, CF_STATUS | CF_MEDIA_POSITION);

	m_mediaObject.setCurrentSource(iqt::GetQString(url));

	if (autoPlay){
		m_mediaObject.play();
	}
	else{
		m_mediaObject.pause();
	}

	return true;
}


void CPhononVideoViewGuiComp::CloseMedium()
{
	istd::CChangeNotifier notifier(this, CF_STATUS);

	m_mediaObject.stop();
	m_mediaObject.clearQueue();
}


bool CPhononVideoViewGuiComp::IsPlaying() const
{
	EnsureSync();

	Phonon::State state = m_mediaObject.state();

	return (state == Phonon::PlayingState) || (state == Phonon::BufferingState);
}


bool CPhononVideoViewGuiComp::SetPlaying(bool state)
{
	istd::CChangeNotifier notifier(this, CF_STATUS);

	if (state){
		m_mediaObject.play();
	}
	else{
		m_mediaObject.pause();
	}

	return true;
}


double CPhononVideoViewGuiComp::GetMediumLength() const
{
	EnsureSync();

	return m_mediaObject.totalTime() * 0.001;
}


double CPhononVideoViewGuiComp::GetCurrentPosition() const
{
	return m_currentPosition;
}


bool CPhononVideoViewGuiComp::SetCurrentPosition(double position)
{
	if (fabs(position - GetCurrentPosition()) < I_BIG_EPSILON){
		return true;
	}

	if (m_mediaObject.isSeekable()){
		istd::CChangeNotifier notifier(this, CF_MEDIA_POSITION);

		m_mediaObject.seek(qint64(position * 1000));

		m_currentPosition = position;

		return true;
	}

	return false;
}


int CPhononVideoViewGuiComp::GetSupportedFeatures() const
{
	int flags = SF_OPEN_MEDIA | SF_PLAY | SF_AUTO_PLAY;

	if (m_mediaObject.isSeekable()){
		flags = flags | SF_SEEK;
	}

	return flags;
}


// reimplemented (imm::IVideoInfo)

int CPhononVideoViewGuiComp::GetFramesCount() const
{
	return int(GetMediumLength() * *m_framesPerSecondAttrPtr);
}


double CPhononVideoViewGuiComp::GetFrameIntervall() const
{
	return 1.0 / *m_framesPerSecondAttrPtr;
}


istd::CIndex2d CPhononVideoViewGuiComp::GetFrameSize() const
{
	return istd::CIndex2d::GetInvalid();
}


double CPhononVideoViewGuiComp::GetPixelAspectRatio() const
{
	return -1;
}


// reimplemented (imm::IVideoController)

int CPhononVideoViewGuiComp::GetCurrentFrame() const
{
	return int(GetCurrentPosition() * *m_framesPerSecondAttrPtr + 0.5);
}


bool CPhononVideoViewGuiComp::SetCurrentFrame(int frameIndex)
{
	return SetCurrentPosition(frameIndex / *m_framesPerSecondAttrPtr);
}


// protected methods

void CPhononVideoViewGuiComp::EnsureSync() const
{
	for (;;){
		int state = m_mediaObject.state();
		if ((state == Phonon::BufferingState) || (state == Phonon::LoadingState)){
			QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
		}
		else{
			break;
		}
	}
}


// reimplemented (iqtgui::CGuiComponentBase)

void CPhononVideoViewGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	Phonon::createPath(&m_mediaObject, GetQtWidget());
}


void CPhononVideoViewGuiComp::OnGuiDestroyed()
{
	CloseMedium();

	BaseClass::OnGuiDestroyed();
}


} // namespace iqtmm


