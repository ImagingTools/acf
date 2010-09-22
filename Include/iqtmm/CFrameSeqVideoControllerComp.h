#ifndef iqtmm_CFrameSeqVideoControllerComp_included
#define iqtmm_CFrameSeqVideoControllerComp_included


// Qt includes
#include <QObject>
#include <QTimer>

#include "iser/IFileLoader.h"

#include "imod/CSingleModelObserverBase.h"

#include "icomp/CComponentBase.h"

#include "iproc/IBitmapAcquisition.h"
#include "iproc/TSyncProcessorCompBase.h"

#include "imm/IVideoController.h"

#include "iqt/CFileList.h"


namespace iqtmm
{


/**
	Component to play some image (frame) sequence from a directory.
*/
class CFrameSeqVideoControllerComp:
			public QObject,
			public iproc::TSyncProcessorCompBase<iproc::IBitmapAcquisition>,
			virtual public imm::IVideoController,
			protected imod::CSingleModelObserverBase
{
	Q_OBJECT

public:
	typedef iproc::TSyncProcessorCompBase<iproc::IBitmapAcquisition> BaseClass;

	I_BEGIN_COMPONENT(CFrameSeqVideoControllerComp);
		I_REGISTER_INTERFACE(istd::IChangeable);
		I_REGISTER_INTERFACE(imm::IMediaController);
		I_REGISTER_INTERFACE(imm::IVideoInfo);
		I_REGISTER_INTERFACE(imm::IVideoController);
		I_ASSIGN(m_framesPerSecondAttrPtr, "FramesPerSecond", "Default number of frames per second if this info is unavailable from video", true, 25.0);
		I_ASSIGN(m_frameLoaderCompPtr, "FrameLoader", "Loader for the single frame", true, "FrameLoader");
		I_ASSIGN(m_frameDataCompPtr, "FrameData", "Frame instance will be loaded with loader", true, "FrameData");
	I_END_COMPONENT();

	CFrameSeqVideoControllerComp();

	// reimplemented (iproc::IBitmapAcquisition)
	virtual istd::CIndex2d GetBitmapSize(const iprm::IParamsSet* paramsPtr) const;

	// reimplemented (iproc::IProcessor)
	virtual int DoProcessing(
				const iprm::IParamsSet* paramsPtr,
				const istd::IPolymorphic* inputPtr,
				istd::IChangeable* outputPtr,
				iproc::IProgressManager* progressManagerPtr = NULL);

	// reimplemented (imm::IMediaController)
	virtual istd::CString GetOpenedMediumUrl() const;
	virtual bool OpenMediumUrl(const istd::CString& url, bool autoPlay = true);
	virtual void CloseMedium();
	virtual bool IsPlaying() const;
	virtual bool SetPlaying(bool state = true);
	virtual double GetMediumLength() const;
	virtual double GetCurrentPosition() const;
	virtual bool SetCurrentPosition(double position);
	virtual int GetSupportedFeatures() const;

	// reimplemented (imm::IVideoInfo)
	virtual int GetFramesCount() const;
	virtual double GetFrameIntervall() const;
	virtual istd::CIndex2d GetFrameSize() const;
	virtual double GetPixelAspectRatio() const;

	// reimplemented (imm::IVideoController)
	virtual int GetCurrentFrame() const;
	virtual bool SetCurrentFrame(int frameIndex);

protected:
	bool LoadCurrentFrame();

private:
	I_ATTR(istd::CString, m_defaultSequenceDirectoryAttrPtr);
	I_ATTR(double, m_framesPerSecondAttrPtr);
	I_REF(iser::IFileLoader, m_frameLoaderCompPtr);
	I_REF(istd::IChangeable, m_frameDataCompPtr);

	int m_currentFrameIndex;

	istd::CString m_mediumUrl;

	iqt::CFileList m_fileList;
	bool m_isPlaying;

	bool m_isFrameLoaded;
};


} // namespace iqtmm


#endif // !iqtmm_CFrameSeqVideoControllerComp_included

