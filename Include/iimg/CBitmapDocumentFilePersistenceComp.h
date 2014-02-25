#ifndef ifile_CBitmapDocumentFilePersistenceComp_included
#define ifile_CBitmapDocumentFilePersistenceComp_included


// ACF includes
#include "ifile/IFilePersistence.h"
#include "ilog/TLoggerCompWrap.h"
#include "iimg/CMultiPageBitmapBase.h"


namespace iimg
{


/**
	File based persistence of the multi-paged bitmap document.

	Document persistence is realized as a list of bitmap files (for each document page) and meta info description file.
	There are three operating modes for this component (controlled by component attribute 'OpeationMode'):
	1. Page bitmaps and meta info are saved in the target directory with following naming convention: <DocumentFileName>_[PageIndex].<DocumentExtension> for bitmap pages and <DocumentFileName>.bdm for the document's meta info.
	2. All files (page bitmaps and meta info) are saved in the document's own directory with the same name as a document file name: <DocumentFileName>.bid
	3. The same as 2. but the document's directory will be zipped. 
*/
class CBitmapDocumentFilePersistenceComp:
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

	I_BEGIN_COMPONENT(CBitmapDocumentFilePersistenceComp);
		I_REGISTER_INTERFACE(ifile::IFileTypeInfo);
		I_REGISTER_INTERFACE(ifile::IFilePersistence);
		I_ASSIGN(m_bitmapPersistenceCompPtr, "BitmapPersistence", "Component used for persistence of the single page of the bitmap document", true, "BitmapPersistence");
		I_ASSIGN(m_operationModeAttrPtr, "OperationMode", "Operation mode", true, OM_COMPRESSED_FOLDER);
	I_END_COMPONENT;

	// reimplemented (ifile::IFilePersistence)
	virtual bool IsOperationSupported(
		const istd::IChangeable* dataObjectPtr,
		const QString* filePathPtr = NULL,
		int flags = -1,
		bool beQuiet = true) const;
	virtual int LoadFromFile(istd::IChangeable& data, const QString& filePath = QString()) const;
	virtual int SaveToFile(const istd::IChangeable& data, const QString& filePath = QString()) const;

	// reimplemented (ifile::IFileTypeInfo)
	virtual bool GetFileExtensions(QStringList& result, int flags = -1, bool doAppend = false) const;
	virtual QString GetTypeDescription(const QString* extensionPtr = NULL) const;

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

protected:
	virtual bool SerializeDocumentMetaInfo(iimg::CMultiPageBitmapBase& document, iser::IArchive& archive) const;
	virtual bool SerializePageMetaInfo(iimg::CMultiPageBitmapBase& document, int pageIndex, iser::IArchive& archive) const;

private:
	I_REF(ifile::IFilePersistence, m_bitmapPersistenceCompPtr);
	I_ATTR(int, m_operationModeAttrPtr);

	QString m_defaultSuffix;
	QString m_defaultPageSuffix;
};


} // namespace iimg


#endif // !ifile_CBitmapDocumentFilePersistenceComp_included


