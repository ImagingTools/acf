#include "iqtmm/CFrameSeqVideoControllerComp.h"


// ACF includes
#include "istd/TChangeNotifier.h"


namespace iqtmm
{


// public methods

CFrameSeqVideoControllerComp::CFrameSeqVideoControllerComp()
:	m_currentFrameIndex(-1),
	m_isFrameLoaded(false)
{
}


// reimplemented (imm::IMediaController)

istd::CString CFrameSeqVideoControllerComp::GetOpenedMediumUrl() const
{
	return m_mediumUrl;
}


bool CFrameSeqVideoControllerComp::OpenMediumUrl(const istd::CString& url, bool autoPlay)
{
	istd::CChangeNotifier notifier(this, CF_STATUS);

	m_mediumUrl = url;

	istd::CStringList fileExtensions;
	
	m_frameLoaderCompPtr->GetFileExtensions(fileExtensions);

	QStringList fileFilter;
	for (int fileExtensionIndex = 0; fileExtensionIndex < int(fileExtensions.size()); fileExtensionIndex++){
		fileFilter.push_back(QString("*.") + iqt::GetQString(fileExtensions[fileExtensionIndex]));
	}
	
	m_fileList.Create(iqt::GetQString(m_mediumUrl), 0, 0, fileFilter);

	SetCurrentFrame(-1);

	SetPlaying(autoPlay);

	return true;
}


void CFrameSeqVideoControllerComp::CloseMedium()
{
	istd::CChangeNotifier notifier(this, CF_STATUS);

	SetPlaying(false);
}


bool CFrameSeqVideoControllerComp::IsPlaying() const
{
	return m_isPlaying;
}


bool CFrameSeqVideoControllerComp::SetPlaying(bool state)
{
	istd::CChangeNotifier notifier(this, CF_STATUS);

	if (m_isPlaying != state){
		m_isPlaying = state;
	}

	return true;
}


double CFrameSeqVideoControllerComp::GetMediumLength() const
{
	return 0;
}


double CFrameSeqVideoControllerComp::GetCurrentPosition() const
{
	return GetCurrentFrame() / *m_framesPerSecondAttrPtr;
}


bool CFrameSeqVideoControllerComp::SetCurrentPosition(double position)
{
	return SetCurrentFrame(position * *m_framesPerSecondAttrPtr + 0.5);
}


int CFrameSeqVideoControllerComp::GetSupportedFeatures() const
{
	return SF_PLAY | SF_SEEK;
}


// reimplemented (imm::IVideoInfo)

int CFrameSeqVideoControllerComp::GetFramesCount() const
{
	return m_fileList.count();
}


double CFrameSeqVideoControllerComp::GetFrameIntervall() const
{
	return 1.0 / *m_framesPerSecondAttrPtr;
}


istd::CIndex2d CFrameSeqVideoControllerComp::GetFrameSize() const
{
	return istd::CIndex2d::GetInvalid();
}


double CFrameSeqVideoControllerComp::GetPixelAspectRatio() const
{
	return 1;
}


// reimplemented (imm::IVideoController)

int CFrameSeqVideoControllerComp::GetCurrentFrame() const
{
	return m_currentFrameIndex;
}


bool CFrameSeqVideoControllerComp::SetCurrentFrame(int frameIndex)
{
	if (frameIndex != m_currentFrameIndex){
		istd::CChangeNotifier notifier(this, CF_MEDIA_POSITION);

		m_currentFrameIndex = frameIndex;

		m_isFrameLoaded = LoadCurrentFrame();
	}

	return m_isFrameLoaded;
}


bool CFrameSeqVideoControllerComp::GrabFrame(iimg::IBitmap& result, int frameIndex) const
{
	if (frameIndex < 0){
		frameIndex = m_currentFrameIndex;
	}

	if (m_isFrameLoaded && (frameIndex == m_currentFrameIndex)){
		const iimg::IRasterImage* imagePtr = dynamic_cast<const iimg::IRasterImage*>(m_frameDataCompPtr.GetPtr());
		if ((imagePtr != NULL) && result.CopyImageFrom(*imagePtr)){
			return true;
		}
	}

	if (m_frameLoaderCompPtr.IsValid() && (frameIndex < m_fileList.count())){
		QString fileName = m_fileList[frameIndex];

		istd::CChangeNotifier notifier(&result);

		int loadState = m_frameLoaderCompPtr->LoadFromFile(result, iqt::GetCString(fileName));

		return (loadState == iser::IFileLoader::StateOk);
	}

	return false;
}


// protected methods

bool CFrameSeqVideoControllerComp::LoadCurrentFrame()
{
	// reset start frame:
	if (m_currentFrameIndex < 0){
		iimg::IRasterImage* imagePtr = dynamic_cast<iimg::IRasterImage*>(m_frameDataCompPtr.GetPtr());
		if (imagePtr != NULL){
			istd::CChangeNotifier changePtr(imagePtr);

			imagePtr->ResetImage();
		}

		return true;
	}

	if (m_frameLoaderCompPtr.IsValid() && m_frameDataCompPtr.IsValid() && (m_currentFrameIndex < m_fileList.count())){
		QString currentFile = m_fileList[m_currentFrameIndex];

		int loadState = m_frameLoaderCompPtr->LoadFromFile(*m_frameDataCompPtr, iqt::GetCString(currentFile));

		return (loadState == iser::IFileLoader::StateOk);
	}
	else{
		return false;
	}
}


} // namespace iqtmm


