#include "iqt/CRenderedObjectFileLoaderComp.h"


// Qt includes
#include <QFileInfo>


// ACF includes
#include "istd/TChangeNotifier.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace iqt
{


// public methods

// reimplemented (iser::IFileLoader)

bool CRenderedObjectFileLoaderComp::IsOperationSupported(
				const istd::IChangeable* dataObjectPtr,
				const istd::CString* filePathPtr,
				int flags,
				bool beQuiet) const
{
	if (m_fileLoaderCompPtr.IsValid()){
		return m_fileLoaderCompPtr->IsOperationSupported(dataObjectPtr, filePathPtr, flags, beQuiet);
	}

	return false;
}


int CRenderedObjectFileLoaderComp::LoadFromFile(istd::IChangeable& data, const istd::CString& filePath) const
{
	iimg::IBitmap* bitmapPtr = dynamic_cast<iimg::IBitmap*>(&data);
	if (bitmapPtr == NULL){
		return StateFailed;
	}

	if (m_fileLoaderCompPtr.IsValid() && m_fileDataCompPtr.IsValid() && m_objectSnapCompPtr.IsValid()){
		QFileInfo fileInfo(iqt::GetQString(filePath));
		if (fileInfo.exists()){
			QDateTime fileTimeStamp = fileInfo.lastModified();

			PreviewCache::iterator foundCacheIter = m_previewCache.find(iqt::GetQString(filePath));
			if (foundCacheIter != m_previewCache.end()){
				if (fileTimeStamp == foundCacheIter->second.fileTimeStamp){
					if (bitmapPtr->CopyFrom(*foundCacheIter->second.fileBitmapPtr.GetPtr())){
						return StateOk;
					}

					return StateFailed;
				}
				else{
					m_previewCache.erase(foundCacheIter);
				}
			}

			int loadResult = m_fileLoaderCompPtr->LoadFromFile(*m_fileDataCompPtr.GetPtr(), filePath);
			if (loadResult == StateOk){
				istd::CChangeNotifier changePtr(bitmapPtr);

				if (m_objectSnapCompPtr->GetSnap(*m_fileDataCompPtr.GetPtr(), *bitmapPtr, istd::CIndex2d(*m_widthAttrPtr, *m_heightAttrPtr))){
					FileInfo fileInfo;
					fileInfo.fileTimeStamp = fileTimeStamp;

					istd::TSmartPtr<iqt::CBitmap> cachedBitmapPtr(new iqt::CBitmap);

					if (cachedBitmapPtr->CopyFrom(*bitmapPtr)){
						fileInfo.fileBitmapPtr = cachedBitmapPtr;

						if (m_maxCacheSizeAttrPtr.IsValid() && (int(m_previewCache.size()) >= *m_maxCacheSizeAttrPtr)){
							m_previewCache.erase(m_previewCache.begin());
						}

						m_previewCache[iqt::GetQString(filePath)] = fileInfo;
					}

					return StateOk;
				}
			}
		}
	}

	return StateFailed;
}


int CRenderedObjectFileLoaderComp::SaveToFile(const istd::IChangeable& data, const istd::CString& filePath) const
{
	if (m_fileLoaderCompPtr.IsValid()){
		return m_fileLoaderCompPtr->SaveToFile(data, filePath);
	}

	return iser::IFileLoader::StateFailed;
}


// reimplemented (iser::IFileTypeInfo)

bool CRenderedObjectFileLoaderComp::GetFileExtensions(istd::CStringList& result, int flags, bool doAppend) const
{
	if (m_fileLoaderCompPtr.IsValid()){
		return m_fileLoaderCompPtr->GetFileExtensions(result, flags, doAppend);
	}

	return false;
}


istd::CString CRenderedObjectFileLoaderComp::GetTypeDescription(const istd::CString* extensionPtr) const
{
	if (m_fileLoaderCompPtr.IsValid()){
		return m_fileLoaderCompPtr->GetTypeDescription(extensionPtr);
	}

	return istd::CString::GetEmpty();
}

	
// reimplemented (iser::ISerializable)

bool CRenderedObjectFileLoaderComp::Serialize(iser::IArchive& archive)
{
	int cacheSize = m_previewCache.size();

	iser::CArchiveTag previewCacheTag("PreviewCacheElements", "Elements list in the preview cache");

	iser::CArchiveTag previewCacheElementTag("PreviewCacheElement", "An element in the preview cache");

	bool retVal = archive.BeginMultiTag(previewCacheTag, previewCacheElementTag, cacheSize);

	if (archive.IsStoring()){
		for (PreviewCache::const_iterator index = m_previewCache.begin(); index != m_previewCache.end(); index++){
			retVal = retVal && archive.BeginTag(previewCacheElementTag);
	
			iser::CArchiveTag filePathTag("FilePath", "Path of the rendered file");
			retVal = retVal && archive.BeginTag(filePathTag);
			istd::CString filePath = iqt::GetCString(index->first);
			retVal = retVal && archive.Process(filePath);
			retVal = retVal && archive.EndTag(filePathTag);

			iser::CArchiveTag fileTimeStampTag("TimeStamp", "Time stamp of the last file update");
			retVal = retVal && archive.BeginTag(fileTimeStampTag);
			isys::CSimpleDateTime timeStamp = iqt::GetCSimpleDateTime(index->second.fileTimeStamp);
			retVal = retVal && timeStamp.Serialize(archive);
			retVal = retVal && archive.EndTag(fileTimeStampTag);

			iser::CArchiveTag bitmapTag("Bitmap", "Rendered bitmap");
			retVal = retVal && archive.BeginTag(bitmapTag);
			retVal = retVal && index->second.fileBitmapPtr->Serialize(archive);
			retVal = retVal && archive.EndTag(bitmapTag);

			retVal = retVal && archive.EndTag(previewCacheElementTag);
		}
	}
	else{
		for (int elementIndex = 0; elementIndex < cacheSize; elementIndex++){
			retVal = retVal && archive.BeginTag(previewCacheElementTag);
	
			iser::CArchiveTag filePathTag("FilePath", "Path of the rendered file");
			retVal = retVal && archive.BeginTag(filePathTag);
			istd::CString filePath;
			retVal = retVal && archive.Process(filePath);
			retVal = retVal && archive.EndTag(filePathTag);

			iser::CArchiveTag fileTimeStampTag("TimeStamp", "Time stamp of the last file update");
			retVal = retVal && archive.BeginTag(fileTimeStampTag);
			isys::CSimpleDateTime timeStamp;
			retVal = retVal && timeStamp.Serialize(archive);
			retVal = retVal && archive.EndTag(fileTimeStampTag);

			istd::TSmartPtr<iqt::CBitmap> bitmapPtr(new iqt::CBitmap);
			iser::CArchiveTag bitmapTag("Bitmap", "Rendered bitmap");
			retVal = retVal && archive.BeginTag(bitmapTag);
			retVal = retVal && bitmapPtr->Serialize(archive);
			retVal = retVal && archive.EndTag(bitmapTag);

			retVal = retVal && archive.EndTag(previewCacheElementTag);

			if (retVal){
				FileInfo fileInfo;

				fileInfo.fileBitmapPtr = bitmapPtr;
				fileInfo.fileTimeStamp = iqt::GetQDateTime(timeStamp);
				m_previewCache[iqt::GetQString(filePath)] = fileInfo;
			}
		}
	}

	retVal = retVal && archive.EndTag(previewCacheTag);

	return retVal;
}


} // namespace iqt


