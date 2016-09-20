#ifndef idoc_CMultiPageDocumentFilePersistenceComp_included
#define idoc_CMultiPageDocumentFilePersistenceComp_included


// ACF includes
#include <ifile/IFilePersistence.h>
#include <ilog/TLoggerCompWrap.h>
#include <idoc/IMultiPageDocument.h>


namespace idoc
{


/**
	File based persistence of the multi-paged bitmap document.

	Document persistence is realized as a list of document page	 files (for each document page) and meta info description file.
	There are three operating modes for this component (controlled by component attribute 'OpeationMode'):
	1. Document pages and meta info are saved in the target directory with following naming convention: <DocumentFileName>_[PageIndex].<DocumentExtension> for bitmap pages and <DocumentFileName>.bdm for the document's meta info.
	2. All files (document pages and meta info) are saved in the document's own directory with the same name as a document file name: <DocumentFileName>.bid
	3. The same as 2. but the document's directory will be zipped. 
*/
class CMultiPageDocumentFilePersistenceComp:
			public ilog::CLoggerComponentBase,
			virtual public ifile::IFilePersistence
{
public:
	typedef ilog::CLoggerComponentBase BaseClass;

	enum OperationMode
	{
		OM_FLAT,
		OM_FOLDER,
		OM_COMPRESSED_FOLDER
	};

	I_BEGIN_COMPONENT(CMultiPageDocumentFilePersistenceComp);
		I_REGISTER_INTERFACE(ifile::IFileTypeInfo);
		I_REGISTER_INTERFACE(ifile::IFilePersistence);
		I_ASSIGN(m_pageObjectPersistenceCompPtr, "BitmapPersistence", "Component used for persistence of the single page of the bitmap document", true, "BitmapPersistence");
		I_ASSIGN(m_operationModeAttrPtr, "OperationMode", "Operation mode. 0 - flat structure, all files will be placed into the target folder\n1 - Files will be placed into the document's own folder\n2 - Same as 1, but the folder wll be compressed", true, OM_FOLDER);
		I_ASSIGN(m_defaultPageSuffixAttrPtr, "DefaultPageFileExtension", "Default extension used for the page files", false, "");	
	I_END_COMPONENT;

	// reimplemented (ifile::IFilePersistence)
	virtual bool IsOperationSupported(
		const istd::IChangeable* dataObjectPtr,
		const QString* filePathPtr = NULL,
		int flags = -1,
		bool beQuiet = true) const;
	virtual int LoadFromFile(
				istd::IChangeable& data,
				const QString& filePath = QString(),
				ibase::IProgressManager* progressManagerPtr = NULL) const;
	virtual int SaveToFile(
				const istd::IChangeable& data,
				const QString& filePath = QString(),
				ibase::IProgressManager* progressManagerPtr = NULL) const;

	// reimplemented (ifile::IFileTypeInfo)
	virtual bool GetFileExtensions(QStringList& result, const istd::IChangeable* dataObjectPtr = NULL, int flags = -1, bool doAppend = false) const;
	virtual QString GetTypeDescription(const QString* extensionPtr = NULL) const;

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

protected:
	virtual bool SerializeDocumentMetaInfo(const idoc::IMultiPageDocument& document, iser::IArchive& archive) const;
	virtual bool SerializePageMetaInfo(idoc::IMultiPageDocument& document, int pageIndex, iser::IArchive& archive) const;
private:
	/**
		Get the path of the bundle info file.
	*/
	QString GetInfoFilePath(const QString documentFilePath) const;

private:
	I_REF(ifile::IFilePersistence, m_pageObjectPersistenceCompPtr);
	I_ATTR(int, m_operationModeAttrPtr);
	I_ATTR(QByteArray, m_defaultPageSuffixAttrPtr);

	QString m_defaultSuffix;
	QString m_defaultPageSuffix;

	static iser::CArchiveTag s_pagesTag;
	static iser::CArchiveTag s_pageFileTag;
};


} // namespace idoc


#endif // !idoc_CMultiPageDocumentFilePersistenceComp_included


