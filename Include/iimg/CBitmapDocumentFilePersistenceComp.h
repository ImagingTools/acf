#ifndef ifile_CBitmapDocumentFilePersistenceComp_included
#define ifile_CBitmapDocumentFilePersistenceComp_included


// ACF includes
#include "ifile/IFilePersistence.h"
#include "ilog/TLoggerCompWrap.h"


namespace iimg
{


class CBitmapDocumentFilePersistenceComp:
			public ilog::CLoggerComponentBase,
			virtual public ifile::IFilePersistence
{
public:
	typedef ilog::CLoggerComponentBase BaseClass;

	I_BEGIN_COMPONENT(CBitmapDocumentFilePersistenceComp);
		I_REGISTER_INTERFACE(ifile::IFileTypeInfo);
		I_REGISTER_INTERFACE(ifile::IFilePersistence);
		I_ASSIGN(m_bitmapPersistenceCompPtr, "BitmapPersistence", "Component used for persistence of the single page of the bitmap document", true, "BitmapPersistence");
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

private:
	I_REF(ifile::IFilePersistence, m_bitmapPersistenceCompPtr);
};


} // namespace iimg


#endif // !ifile_CBitmapDocumentFilePersistenceComp_included
