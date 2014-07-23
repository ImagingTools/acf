#include "iimg/CBitmapDocumentFilePersistenceComp.h"


// Qt includes
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QDir>

// ACF includes
#include "iimg/CBitmapLoaderComp.h"
#include "ifile/CXmlFileReadArchive.h"
#include "ifile/CXmlFileWriteArchive.h"
#include "idoc/CStandardDocumentMetaInfo.h"


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
	if (dataObjectPtr != NULL && dynamic_cast<const iimg::CMultiPageBitmapBase*>(dataObjectPtr) == NULL){
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


static iser::CArchiveTag pagesTag("Pages", "Container of the document pages", iser::CArchiveTag::TT_MULTIPLE);
static iser::CArchiveTag pageFileTag("PageFile", "Single document page", iser::CArchiveTag::TT_LEAF, &pagesTag);


int CBitmapDocumentFilePersistenceComp::LoadFromFile(
			istd::IChangeable& data,
			const QString& filePath,
			ibase::IProgressManager* /*progressManagerPtr*/) const
{
	if (!m_bitmapPersistenceCompPtr.IsValid()){
		SendCriticalMessage(0, "Bad component configuration! Component 'BitmapPersistence' was not set");

		return OS_FAILED;
	}

	iimg::CMultiPageBitmapBase* docPtr = dynamic_cast<iimg::CMultiPageBitmapBase*>(&data);
	if (docPtr == NULL){
		return OS_FAILED;
	}

	istd::CChangeNotifier updatePtr(docPtr);

	docPtr->ResetPages();

	if (!QFile::exists(filePath)){
		return OS_FAILED;
	}

	QFileInfo fileInfo(filePath);

	ifile::CXmlFileReadArchive archive(filePath);
	bool retVal = true;

	// Serialize meta info:
	retVal = retVal && SerializeDocumentMetaInfo(*docPtr, archive);

	// Serialize document pages:
	int pagesCount = docPtr->GetPagesCount();

	retVal = retVal && archive.BeginMultiTag(pagesTag, pageFileTag, pagesCount);

	for (int pageIndex = 0; pageIndex < pagesCount; ++pageIndex){
		// load bitmap file
		QString pageFileName;

		retVal = retVal && archive.BeginTag(pageFileTag);
		retVal = retVal && archive.Process(pageFileName);
		retVal = retVal && archive.EndTag(pageFileTag);

		idoc::CStandardDocumentMetaInfo pageMetaInfo;
		pageMetaInfo.SetMetaInfo(idoc::IDocumentMetaInfo::MIT_TITLE, pageFileName);

		iimg::IBitmap* bitmapPtr = dynamic_cast<iimg::IBitmap*>(docPtr->InsertPage(&pageMetaInfo));
		if (bitmapPtr == NULL || pageFileName.isEmpty()){
			retVal = false;
		}
		else{
			QString bitmapFilePath = fileInfo.absolutePath() + "/" + pageFileName;

			int loadState = m_bitmapPersistenceCompPtr->LoadFromFile(*bitmapPtr, bitmapFilePath);

			retVal = retVal && (loadState == ifile::IFilePersistence::OS_OK);
		}

		// load page meta info
		retVal = retVal && SerializePageMetaInfo(*docPtr, pageIndex, archive);
	}

	retVal = retVal && archive.EndTag(pagesTag);

	return retVal ? OS_OK : OS_FAILED;
}


int CBitmapDocumentFilePersistenceComp::SaveToFile(
			const istd::IChangeable& data,
			const QString& filePath,
			ibase::IProgressManager* /*progressManagerPtr*/) const
{
	if (!m_bitmapPersistenceCompPtr.IsValid()){
		SendCriticalMessage(0, "Bad component configuration! Component 'BitmapPersistence' was not set");

		return OS_FAILED;
	}

	const iimg::CMultiPageBitmapBase* docPtr = dynamic_cast<const iimg::CMultiPageBitmapBase*>(&data);
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

	ifile::CXmlFileWriteArchive archive(fileName);
	bool retVal = true;
	
	// Serialize meta info:
	retVal = retVal && SerializeDocumentMetaInfo(const_cast<iimg::CMultiPageBitmapBase&>(*docPtr), archive);

	// Serialize document pages:
	int pagesCount = docPtr->GetPagesCount();

	retVal = retVal && archive.BeginMultiTag(pagesTag, pageFileTag, pagesCount);

	for (int pageIndex = 0; pageIndex < pagesCount; ++pageIndex){
		// write page bitmap
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

		// write page meta info
		retVal = retVal && SerializePageMetaInfo(const_cast<iimg::CMultiPageBitmapBase&>(*docPtr), pageIndex, archive);
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


// protected methods

bool CBitmapDocumentFilePersistenceComp::SerializeDocumentMetaInfo(iimg::CMultiPageBitmapBase& document, iser::IArchive& archive) const
{
	bool retVal = true;

	idoc::CStandardDocumentMetaInfo* metaInfoPtr = dynamic_cast<idoc::CStandardDocumentMetaInfo*>(const_cast<idoc::IDocumentMetaInfo*>(&document.GetDocumentMetaInfo()));
	if (metaInfoPtr != NULL){
		static iser::CArchiveTag metaInfoTag("MetaInfo", "Meta information about the document", iser::CArchiveTag::TT_GROUP);
		retVal = retVal && archive.BeginTag(metaInfoTag);
		retVal = retVal && metaInfoPtr->idoc::CStandardDocumentMetaInfo::Serialize(archive);
		retVal = retVal && archive.EndTag(metaInfoTag);
	}

	return retVal;
}


bool CBitmapDocumentFilePersistenceComp::SerializePageMetaInfo(iimg::CMultiPageBitmapBase& document, int pageIndex, iser::IArchive& archive) const
{
	bool retVal = true;

	iser::ISerializable* metaInfoSerializablePtr = dynamic_cast<iser::ISerializable*>(
		const_cast<idoc::IDocumentMetaInfo*>(document.GetPageMetaInfo(pageIndex))
		);
	if (metaInfoSerializablePtr != NULL){
		static iser::CArchiveTag pageMetaInfoTag("PageMetaInfo", "Single document page meta information", iser::CArchiveTag::TT_GROUP);
		retVal = retVal && archive.BeginTag(pageMetaInfoTag);
		retVal = retVal && metaInfoSerializablePtr->Serialize(archive);
		retVal = retVal && archive.EndTag(pageMetaInfoTag);
	}

	return retVal;
}


} // namespace iimg


