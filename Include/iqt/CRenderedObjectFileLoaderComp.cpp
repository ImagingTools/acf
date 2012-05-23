#include "iqt/CRenderedObjectFileLoaderComp.h"


// Qt includes
#include <QtCore/QFileInfo>

// ACF includes
#include "istd/TChangeNotifier.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"
#include "iser/CPrimitiveTypesSerializer.h"


namespace iqt
{


// public methods

// reimplemented (iser::IFileLoader)

bool CRenderedObjectFileLoaderComp::IsOperationSupported(
				const istd::IChangeable* dataObjectPtr,
				const QString* filePathPtr,
				int flags,
				bool beQuiet) const
{
	if (m_fileLoaderCompPtr.IsValid()){
		return m_fileLoaderCompPtr->IsOperationSupported(dataObjectPtr, filePathPtr, flags, beQuiet);
	}

	return false;
}


int CRenderedObjectFileLoaderComp::LoadFromFile(istd::IChangeable& data, const QString& filePath) const
{
	iimg::IBitmap* bitmapPtr = dynamic_cast<iimg::IBitmap*>(&data);
	if (bitmapPtr == NULL){
		return StateFailed;
	}

	if (m_fileLoaderCompPtr.IsValid() && m_fileDataCompPtr.IsValid() && m_objectSnapCompPtr.IsValid()){
		QFileInfo fileInfo(filePath);
		if (fileInfo.exists()){
			QDateTime fileTimeStamp = fileInfo.lastModified();

			PreviewCache::iterator foundCacheIter = m_previewCache.find(filePath);
			if (foundCacheIter != m_previewCache.end()){
				if (fileTimeStamp == foundCacheIter.value().fileTimeStamp){
					if (bitmapPtr->CopyFrom(*foundCacheIter.value().fileBitmapPtr.GetPtr())){
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

						m_previewCache[filePath] = fileInfo;
					}

					return StateOk;
				}
			}
		}
	}

	return StateFailed;
}


int CRenderedObjectFileLoaderComp::SaveToFile(const istd::IChangeable& data, const QString& filePath) const
{
	if (m_fileLoaderCompPtr.IsValid()){
		return m_fileLoaderCompPtr->SaveToFile(data, filePath);
	}

	return iser::IFileLoader::StateFailed;
}


// reimplemented (iser::IFileTypeInfo)

bool CRenderedObjectFileLoaderComp::GetFileExtensions(QStringList& result, int flags, bool doAppend) const
{
	if (m_fileLoaderCompPtr.IsValid()){
		return m_fileLoaderCompPtr->GetFileExtensions(result, flags, doAppend);
	}

	return false;
}


QString CRenderedObjectFileLoaderComp::GetTypeDescription(const QString* extensionPtr) const
{
	if (m_fileLoaderCompPtr.IsValid()){
		return m_fileLoaderCompPtr->GetTypeDescription(extensionPtr);
	}

	return QString();
}

	
// reimplemented (iser::ISerializable)

bool CRenderedObjectFileLoaderComp::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag previewCacheTag("PreviewCacheElements", "Elements list in the preview cache");
	static iser::CArchiveTag previewCacheElementTag("PreviewCacheElement", "An element in the preview cache");
	static iser::CArchiveTag filePathTag("FilePath", "Path of the rendered file");
	static iser::CArchiveTag fileTimeStampTag("TimeStamp", "Time stamp of the last file update");
	static iser::CArchiveTag bitmapTag("Bitmap", "Rendered bitmap");

	int cacheSize = m_previewCache.size();

	bool retVal = archive.BeginMultiTag(previewCacheTag, previewCacheElementTag, cacheSize);

	if (archive.IsStoring()){
		for (PreviewCache::const_iterator index = m_previewCache.begin(); index != m_previewCache.end(); index++){
			retVal = retVal && archive.BeginTag(previewCacheElementTag);
	
			retVal = retVal && archive.BeginTag(filePathTag);
			QString filePath = index.key();
			retVal = retVal && archive.Process(filePath);
			retVal = retVal && archive.EndTag(filePathTag);

			retVal = retVal && archive.BeginTag(fileTimeStampTag);
			QDateTime fileTimeStamp = index.value().fileTimeStamp;
			retVal = retVal && iser::CPrimitiveTypesSerializer::SerializeDateTime(archive, fileTimeStamp);
			retVal = retVal && archive.EndTag(fileTimeStampTag);

			retVal = retVal && archive.BeginTag(bitmapTag);
			retVal = retVal && index.value().fileBitmapPtr->Serialize(archive);
			retVal = retVal && archive.EndTag(bitmapTag);

			retVal = retVal && archive.EndTag(previewCacheElementTag);
		}
	}
	else{
		istd::CChangeNotifier notifier(this);

		for (int elementIndex = 0; elementIndex < cacheSize; elementIndex++){
			FileInfo fileInfo;

			retVal = retVal && archive.BeginTag(previewCacheElementTag);
	
			retVal = retVal && archive.BeginTag(filePathTag);
			QString filePath;
			retVal = retVal && archive.Process(filePath);
			retVal = retVal && archive.EndTag(filePathTag);

			retVal = retVal && archive.BeginTag(fileTimeStampTag);
			retVal = retVal && iser::CPrimitiveTypesSerializer::SerializeDateTime(archive, fileInfo.fileTimeStamp);
			retVal = retVal && archive.EndTag(fileTimeStampTag);

			istd::TSmartPtr<iqt::CBitmap> bitmapPtr(new iqt::CBitmap);
			retVal = retVal && archive.BeginTag(bitmapTag);
			retVal = retVal && bitmapPtr->Serialize(archive);
			retVal = retVal && archive.EndTag(bitmapTag);

			retVal = retVal && archive.EndTag(previewCacheElementTag);

			if (retVal){
				fileInfo.fileBitmapPtr = bitmapPtr;
				m_previewCache[filePath] = fileInfo;
			}
		}
	}

	retVal = retVal && archive.EndTag(previewCacheTag);

	return retVal;
}


} // namespace iqt


