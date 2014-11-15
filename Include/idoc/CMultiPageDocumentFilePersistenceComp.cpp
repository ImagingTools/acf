#include "idoc/CMultiPageDocumentFilePersistenceComp.h"


// Qt includes
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QDir>

// ACF includes
#include "ifile/CXmlFileReadArchive.h"
#include "ifile/CXmlFileWriteArchive.h"
#include "idoc/CStandardDocumentMetaInfo.h"


namespace idoc
{


// reimplemented (ifile::IFilePersistence)

bool CMultiPageDocumentFilePersistenceComp::IsOperationSupported(
			const istd::IChangeable* dataObjectPtr,
			const QString* filePathPtr,
			int flags,
			bool /*beQuiet*/) const
{
	if (dataObjectPtr != NULL && dynamic_cast<const idoc::IMultiPageDocument*>(dataObjectPtr) == NULL){
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


int CMultiPageDocumentFilePersistenceComp::LoadFromFile(
			istd::IChangeable& data,
			const QString& filePath,
			ibase::IProgressManager* /*progressManagerPtr*/) const
{
	if (!m_pageObjectPersistenceCompPtr.IsValid()){
		SendCriticalMessage(0, "Bad component configuration! Component 'BitmapPersistence' was not set");

		return OS_FAILED;
	}

	idoc::IMultiPageDocument* docPtr = dynamic_cast<idoc::IMultiPageDocument*>(&data);
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

	retVal = retVal && archive.BeginMultiTag(s_pagesTag, s_pageFileTag, pagesCount);

	for (int pageIndex = 0; pageIndex < pagesCount; ++pageIndex){
		// load bitmap file
		QString pageFileName;

		retVal = retVal && archive.BeginTag(s_pageFileTag);
		retVal = retVal && archive.Process(pageFileName);
		retVal = retVal && archive.EndTag(s_pageFileTag);

		idoc::CStandardDocumentMetaInfo pageMetaInfo;
		pageMetaInfo.SetMetaInfo(idoc::IDocumentMetaInfo::MIT_TITLE, pageFileName);

		istd::IChangeable* pageObjectPtr = docPtr->InsertPage(&pageMetaInfo);
		if (pageObjectPtr == NULL || pageFileName.isEmpty()){
			retVal = false;
		}
		else{
			QString bitmapFilePath = fileInfo.absolutePath() + "/" + pageFileName;

			int loadState = m_pageObjectPersistenceCompPtr->LoadFromFile(*pageObjectPtr, bitmapFilePath);

			retVal = retVal && (loadState == ifile::IFilePersistence::OS_OK);
		}

		// load page meta info
		retVal = retVal && SerializePageMetaInfo(*docPtr, pageIndex, archive);
	}

	retVal = retVal && archive.EndTag(s_pagesTag);

	return retVal ? OS_OK : OS_FAILED;
}


int CMultiPageDocumentFilePersistenceComp::SaveToFile(
			const istd::IChangeable& data,
			const QString& filePath,
			ibase::IProgressManager* /*progressManagerPtr*/) const
{
	if (!m_pageObjectPersistenceCompPtr.IsValid()){
		SendCriticalMessage(0, "Bad component configuration! Component 'BitmapPersistence' was not set");

		return OS_FAILED;
	}

	const idoc::IMultiPageDocument* docPtr = dynamic_cast<const idoc::IMultiPageDocument*>(&data);
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
	retVal = retVal && SerializeDocumentMetaInfo(const_cast<idoc::IMultiPageDocument&>(*docPtr), archive);

	// Serialize document pages:
	int pagesCount = docPtr->GetPagesCount();

	retVal = retVal && archive.BeginMultiTag(s_pagesTag, s_pageFileTag, pagesCount);

	for (int pageIndex = 0; pageIndex < pagesCount; ++pageIndex){
		QString pageName;
		const idoc::IDocumentMetaInfo* pageMetaInfoPtr = docPtr->GetPageMetaInfo(pageIndex);
		if (pageMetaInfoPtr != NULL){
			pageName = pageMetaInfoPtr->GetMetaInfo(idoc::IDocumentMetaInfo::MIT_TITLE).toString();
		}

		if (pageName.isEmpty()){
			pageName = QString::number(pageIndex + 1);
		}

		QString pageFileName = QString("%1_%2.%3")
			.arg(filePathInfo.completeBaseName())
			.arg(pageName)
			.arg(m_defaultPageSuffix);

		retVal = retVal && archive.BeginTag(s_pageFileTag);
		retVal = retVal && archive.Process(pageFileName);
		retVal = retVal && archive.EndTag(s_pageFileTag);

		if (retVal){
			int saveState = (m_pageObjectPersistenceCompPtr->SaveToFile(docPtr->GetDocumentPage(pageIndex), outDir.absoluteFilePath(pageFileName)));

			retVal = retVal && (saveState == ifile::IFilePersistence::OS_OK);
		}
		else{
			retVal = false;
		}

		// write page meta info
		retVal = retVal && SerializePageMetaInfo(const_cast<idoc::IMultiPageDocument&>(*docPtr), pageIndex, archive);
	}

	retVal = retVal && archive.EndTag(s_pagesTag);

	return retVal ? OS_OK : OS_FAILED;
}


// reimplemented (ifile::IFileTypeInfo)

bool CMultiPageDocumentFilePersistenceComp::GetFileExtensions(QStringList& result, const istd::IChangeable* /*dataObjectPtr*/, int /*flags*/, bool doAppend) const
{
	if (!doAppend){
		result.clear();
	}

	bool retVal = true;

	result << m_defaultSuffix;

	return retVal;
}


QString CMultiPageDocumentFilePersistenceComp::GetTypeDescription(const QString* extensionPtr) const
{
	if (extensionPtr != NULL){
		if (extensionPtr->toLower() == m_defaultSuffix){
			return "Multi-Page Document";
		}
	}

	return QString();
}


// reimplemented (icomp::CComponentBase)

void CMultiPageDocumentFilePersistenceComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	m_defaultSuffix = "mpd";

	if (m_pageObjectPersistenceCompPtr.IsValid()){
		QStringList bitmapSuffixes;
		if (m_pageObjectPersistenceCompPtr->GetFileExtensions(bitmapSuffixes, NULL, ifile::IFileTypeInfo::QF_FILE)){
			m_defaultPageSuffix = bitmapSuffixes.first();
		}
	}
}


// protected methods

bool CMultiPageDocumentFilePersistenceComp::SerializeDocumentMetaInfo(const idoc::IMultiPageDocument& document, iser::IArchive& archive) const
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


bool CMultiPageDocumentFilePersistenceComp::SerializePageMetaInfo(idoc::IMultiPageDocument& document, int pageIndex, iser::IArchive& archive) const
{
	bool retVal = true;

	iser::ISerializable* metaInfoSerializablePtr = dynamic_cast<iser::ISerializable*>(const_cast<idoc::IDocumentMetaInfo*>(document.GetPageMetaInfo(pageIndex)));
	if (metaInfoSerializablePtr != NULL){
		static iser::CArchiveTag pageMetaInfoTag("PageMetaInfo", "Single document page meta information", iser::CArchiveTag::TT_GROUP);
		retVal = retVal && archive.BeginTag(pageMetaInfoTag);
		retVal = retVal && metaInfoSerializablePtr->Serialize(archive);
		retVal = retVal && archive.EndTag(pageMetaInfoTag);
	}

	return retVal;
}


// private static members

iser::CArchiveTag CMultiPageDocumentFilePersistenceComp::s_pagesTag("Pages", "Container of the document pages", iser::CArchiveTag::TT_MULTIPLE);
iser::CArchiveTag CMultiPageDocumentFilePersistenceComp::s_pageFileTag("PageFile", "Single document page", iser::CArchiveTag::TT_LEAF, &s_pagesTag);



} // namespace idoc


