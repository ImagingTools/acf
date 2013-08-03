#include "iimg/CBitmapDocumentFilePersistenceComp.h"

// Qt includes
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QDir>

// ACF includes
#include "iimg/CBitmapDocument.h"
#include "iimg/CBitmapLoaderComp.h"

#include "iqt/CXmlFileWriteArchive.h"


namespace iimg
{


// reimplemented (ifile::IFilePersistence)

bool CBitmapDocumentFilePersistenceComp::IsOperationSupported(
	const istd::IChangeable* dataObjectPtr,
	const QString* filePathPtr,
	int /*flags*/,
	bool /*beQuiet*/) const
{
	if (dynamic_cast<const iimg::CBitmapDocument*>(dataObjectPtr) == NULL){
		return false;
	}

	if (filePathPtr != NULL){
		if (filePathPtr->isEmpty()){
			return false;
		}
	}

	return true;
}


int CBitmapDocumentFilePersistenceComp::LoadFromFile(istd::IChangeable& data, const QString& filePath) const
{
	if (!m_bitmapPersistenceCompPtr.IsValid()){
		SendCriticalMessage(0, "Bad component configuration! Component 'BitmapPersistence' was not set");

		return OS_FAILED;
	}

	iimg::CBitmapDocument* docPtr = dynamic_cast<iimg::CBitmapDocument*>(&data);
	if (docPtr == NULL){
		return OS_FAILED;
	}

	if (!QFile::exists(filePath)){
		return OS_FAILED;
	}

	// todo
	return OS_FAILED;
}


int CBitmapDocumentFilePersistenceComp::SaveToFile(const istd::IChangeable& data, const QString& filePath) const
{
	if (!m_bitmapPersistenceCompPtr.IsValid()){
		SendCriticalMessage(0, "Bad component configuration! Component 'BitmapPersistence' was not set");

		return OS_FAILED;
	}

	const iimg::CBitmapDocument* docPtr = dynamic_cast<const iimg::CBitmapDocument*>(&data);
	if (docPtr == NULL){
		return OS_FAILED;
	}

	// Ensure that the output path exists:
	if (filePath.isEmpty()){
		return OS_FAILED;
	}

	QFileInfo filePathInfo(filePath);
	QDir outDir(filePathInfo.absolutePath());
	if (!outDir.exists()){
		if (!outDir.mkpath(".")){
			return OS_FAILED;
		}
	}

	iqt::CXmlFileWriteArchive archive(filePath);
	bool retVal = true;
	
	// Serialize meta info:
	static iser::CArchiveTag metaInfoTag("MetaInfo", "Meta information about the document");

	iser::ISerializable* metaInfoSerializablePtr = dynamic_cast<iser::ISerializable*>(const_cast<idoc::IDocumentMetaInfo*>(&docPtr->GetDocumentMetaInfo()));
	if (metaInfoSerializablePtr != NULL){
		static iser::CArchiveTag metaInfoTag("MetaInfo", "Meta information about the document");
		retVal = retVal && archive.BeginTag(metaInfoTag);
		retVal = retVal && metaInfoSerializablePtr->Serialize(archive);
	}

	retVal = retVal && archive.EndTag(metaInfoTag);

	// Serialize document pages:
	static iser::CArchiveTag pagesTag("Pages", "Container of the document pages");
	static iser::CArchiveTag pageFileTag("PageFile", "Single document page");

	int pagesCount = docPtr->GetPagesCount();

	retVal = retVal && archive.BeginMultiTag(pagesTag, pageFileTag, pagesCount);

	for (int pageIndex = 0; pageIndex < pagesCount; ++pageIndex){
		QString pageName = docPtr->GetOptionName(pageIndex);
		if (pageName.isEmpty()){
			pageName = QString::number(pageIndex+1);
		}

		QString pageFileName = QString("%1_%2.bmp").arg(filePathInfo.completeBaseName()).arg(pageName);

		retVal = retVal && archive.BeginTag(pageFileTag);
		retVal = retVal && archive.Process(pageFileName);
		retVal = retVal && archive.EndTag(pageFileTag);

		const iimg::IBitmap* bitmapPtr = docPtr->GetBitmap(pageIndex);
		if ((bitmapPtr != NULL) && retVal){
			int saveState = (m_bitmapPersistenceCompPtr->SaveToFile(*bitmapPtr, outDir.absoluteFilePath(pageFileName)));

			retVal = retVal && (saveState == ifile::IFilePersistence::OS_OK);
		}
		else{
			retVal = false;
		}
	}

	retVal = retVal && archive.EndTag(pagesTag);

	return retVal ? OS_OK : OS_FAILED;
}


// reimplemented (ifile::IFileTypeInfo)

bool CBitmapDocumentFilePersistenceComp::GetFileExtensions(QStringList& result, int /*flags*/, bool doAppend) const
{
	if (!doAppend){
		result.clear();
	}

	bool retVal = true;

	result << "bdm";

	return retVal;
}


QString CBitmapDocumentFilePersistenceComp::GetTypeDescription(const QString* extensionPtr) const
{
	if (extensionPtr != NULL){
		if (extensionPtr->toLower() == "bdm"){
			return "Bitmap Document Meta Information";
		}
	}

	return QString();
}


} // namespace iimg


