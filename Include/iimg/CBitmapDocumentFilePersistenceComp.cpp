#include "iimg/CBitmapDocumentFilePersistenceComp.h"

// Qt includes
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QDir>

// ACF includes
#include "iimg/CBitmapDocument.h"
#include "iimg/CBitmapLoaderComp.h"

#include "iqt/CXmlFileReadArchive.h"
#include "iqt/CXmlFileWriteArchive.h"


#define DOCUMENT_SUFFIX "mbd"


namespace iimg
{


// reimplemented (ifile::IFilePersistence)

bool CBitmapDocumentFilePersistenceComp::IsOperationSupported(
	const istd::IChangeable* dataObjectPtr,
	const QString* filePathPtr,
	int flags,
	bool /*beQuiet*/) const
{
	if (dataObjectPtr != NULL && dynamic_cast<const iimg::CBitmapDocument*>(dataObjectPtr) == NULL){
		return false;
	}

	if (filePathPtr != NULL){
		if (filePathPtr->isEmpty()){
			return false;
		}
	}

	if (flags & (ifile::IFilePersistence::QF_DIRECTORY)){
		return false;
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

	docPtr->ResetPages();

	if (!QFile::exists(filePath)){
		return OS_FAILED;
	}

	QFileInfo fileInfo(filePath);

	iqt::CXmlFileReadArchive archive(filePath);
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
		QString pageFileName;

		retVal = retVal && archive.BeginTag(pageFileTag);
		retVal = retVal && archive.Process(pageFileName);
		retVal = retVal && archive.EndTag(pageFileTag);

		iimg::CBitmapDocument::Bitmap* bitmapPtr = 
			dynamic_cast<iimg::CBitmapDocument::Bitmap*>(docPtr->InsertPage(pageFileName));

		if (bitmapPtr == NULL || pageFileName.isEmpty()){
			retVal = false;
		} else {
			int loadState = m_bitmapPersistenceCompPtr->LoadFromFile(
				*bitmapPtr, 
				fileInfo.absolutePath() + "/" + pageFileName);

			retVal = retVal && (loadState == ifile::IFilePersistence::OS_OK);
		}
	}

	retVal = retVal && archive.EndTag(pagesTag);

	return retVal ? OS_OK : OS_FAILED;
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

	// add suffixes
	QString fileName(filePath);
	if (filePathInfo.suffix().toLower() != m_defaultSuffix.toLower()){
		fileName += "." + m_defaultSuffix;
	}

	iqt::CXmlFileWriteArchive archive(fileName);
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

		QString pageFileName = QString("%1_%2.%3")
			.arg(filePathInfo.completeBaseName())
			.arg(pageName)
			.arg(m_defaultPageSuffix);

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

	result << DOCUMENT_SUFFIX;

	return retVal;
}


QString CBitmapDocumentFilePersistenceComp::GetTypeDescription(const QString* extensionPtr) const
{
	if (extensionPtr != NULL){
		if (extensionPtr->toLower() == DOCUMENT_SUFFIX){
			return "Multi Bitmap Document";
		}
	}

	return QString();
}


// reimplemented (icomp::CComponentBase)

void CBitmapDocumentFilePersistenceComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	m_defaultSuffix = DOCUMENT_SUFFIX;
	m_defaultPageSuffix = "bmp";

	if (m_bitmapPersistenceCompPtr.IsValid()){
		QStringList bitmapSuffixes;
		if (m_bitmapPersistenceCompPtr->GetFileExtensions(bitmapSuffixes, ifile::IFileTypeInfo::QF_FILE)){
			m_defaultPageSuffix = bitmapSuffixes.first();
		}
	}
}


} // namespace iimg


