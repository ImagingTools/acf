#include "iqt/CRenderedObjectFileLoaderComp.h"


// Qt includes
#include <QFileInfo>


// ACF includes
#include "istd/TChangeNotifier.h"


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


} // namespace iqt


