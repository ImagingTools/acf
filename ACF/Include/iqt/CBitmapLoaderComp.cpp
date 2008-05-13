#include "iqt/CBitmapLoaderComp.h"


// Qt includes
#include <QFileInfo>
#include <QByteArray>
#include <QImageReader>

#include "istd/TChangeNotifier.h"

#include "iproc/IParamsSet.h"

#include "iqt/IQImageProvider.h"


namespace iqt
{


CBitmapLoaderComp::CBitmapLoaderComp()
:	m_lastIdStamp(0)
{
}


// reimplemented (iser::IFileLoader)

bool CBitmapLoaderComp::IsOperationSupported(
			const istd::IChangeable* dataObjectPtr,
			const istd::CString* filePathPtr,
			bool forLoading,
			bool forSaving) const
{
	if ((dataObjectPtr != NULL) && (dynamic_cast<const IQImageProvider*>(dataObjectPtr) == NULL)){
		return false;
	}

	if (filePathPtr != NULL){
		QFileInfo info(iqt::GetQString(*filePathPtr));
		if (forLoading && !forSaving){
			if (!info.exists()){
				return false;
			}

			QByteArray format = QImageReader::imageFormat(info.filePath());

			return !format.isEmpty();
		}
	}

	return true;
}


int CBitmapLoaderComp::LoadFromFile(istd::IChangeable& data, const istd::CString& filePath) const
{
	IQImageProvider* bitmapPtr = dynamic_cast<IQImageProvider*>(&data);

	if (bitmapPtr != NULL){
		istd::CChangeNotifier notifier(&data);

		QImage image;
		if (image.load(iqt::GetQString(filePath))){
			const_cast<CBitmapLoaderComp*>(this)->SetLastLoadFileName(filePath);

			bitmapPtr->CopyImageFrom(image);

			return StateOk;
		}
	}

	return StateFailed;
}


int CBitmapLoaderComp::SaveToFile(const istd::IChangeable& data, const istd::CString& filePath) const
{
	const IQImageProvider* bitmapPtr = dynamic_cast<const IQImageProvider*>(&data);

	if (bitmapPtr != NULL){
		const QImage& image = bitmapPtr->GetQImage();
		if (image.save(iqt::GetQString(filePath))){
			const_cast<CBitmapLoaderComp*>(this)->SetLastSaveFileName(filePath);

			return StateOk;
		}
	}

	return StateFailed;
}


const istd::CString& CBitmapLoaderComp::GetLastLoadFileName() const
{
	return m_lastLoadFileName;
}


const istd::CString& CBitmapLoaderComp::GetLastSaveFileName() const
{
	return m_lastSaveFileName;
}


// reimplemented (iproc::TSyncProcessorWrap<isys::ITimer, iimg::IBitmap>)

int CBitmapLoaderComp::DoSyncProcess(const iproc::IParamsSet* paramsPtr, const isys::ITimer* /*inputPtr*/, iimg::IBitmap* outputPtr)
{
	I_ASSERT(m_defaultDirAttrPtr.IsValid());	// obligatory attribute
	I_ASSERT(m_parameterIdAttrPtr.IsValid());	// obligatory attribute
	I_ASSERT(m_maxCachedDirectoriesAttrPtr.IsValid());	// obligatory attribute

	const icam::IFileBitmapAcquisitionParams* loaderParamsPtr = NULL;
	if (paramsPtr != NULL){
		loaderParamsPtr = dynamic_cast<const icam::IFileBitmapAcquisitionParams*>(paramsPtr->GetParameter((*m_parameterIdAttrPtr).ToString()));
	}

	ParamsInfo& info = m_dirInfos[loaderParamsPtr];
	if (info.files.isEmpty()){
		if (loaderParamsPtr != NULL){
			info.directory = iqt::GetQString(*m_defaultDirAttrPtr);
		}
		else{
			info.directory = iqt::GetQString(loaderParamsPtr->GetDirectory());
		}
	}

	if (info.filesIter == info.files.end()){
		info.files = info.directory.entryList(QDir::Files | QDir::Readable);
		info.filesIter = info.files.begin();
	}

	info.idStamp = ++m_lastIdStamp;

	int retVal = TS_INVALID;
	if (info.filesIter != info.files.end()){
		istd::CString fileName = iqt::GetCString(info.directory.absoluteFilePath(*info.filesIter));
		info.filesIter++;

		if (outputPtr != NULL){
			int loadState = LoadFromFile(*outputPtr, fileName);
			retVal = (loadState == StateOk)? TS_OK: TS_INVALID;
		}
	}

	if (int(m_dirInfos.size()) > *m_maxCachedDirectoriesAttrPtr){
		DirInfos::iterator maxIdStampDiffIter = m_dirInfos.end();
		I_DWORD maxIdStampDiff = 0;
		for (		DirInfos::iterator iter = m_dirInfos.begin();
					iter != m_dirInfos.end();
					++iter){
			I_DWORD idStampDiff = m_lastIdStamp - iter->second.idStamp;
			if (idStampDiff >= maxIdStampDiff){
				maxIdStampDiff = idStampDiff;
				maxIdStampDiffIter = iter;
			}
		}

		if (maxIdStampDiffIter != m_dirInfos.end()){
			m_dirInfos.erase(maxIdStampDiffIter);	// remove element recently not used
		}
	}

	return retVal;
}


// reimplemented (icam::IBitmapAcquisition)

istd::CIndex2d CBitmapLoaderComp::GetBitmapSize(const iproc::IParamsSet* /*paramsPtr*/) const
{
	return istd::CIndex2d(-1, -1);	// unknown size
}


} // namespace iqt


