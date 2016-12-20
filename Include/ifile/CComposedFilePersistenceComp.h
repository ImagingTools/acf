#ifndef ifile_CComposedFilePersistenceComp_included
#define ifile_CComposedFilePersistenceComp_included


// ACF includes
#include <ifile/IFilePersistence.h>
#include <ifile/IFilePersistenceInfo.h>
#include <icomp/CComponentBase.h>

// Qt includes
#include <QtCore/QFileInfo>


namespace ifile
{


/**
	Loader component, which delegates the persistence functionality to its slave loaders.

	\ingroup Persistence
*/
class CComposedFilePersistenceComp:
			public icomp::CComponentBase,
			virtual public ifile::IFilePersistence,
			virtual public ifile::IFilePersistenceInfo
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CComposedFilePersistenceComp);
		I_REGISTER_INTERFACE(ifile::IFileTypeInfo);
		I_REGISTER_INTERFACE(ifile::IFilePersistence);
		I_REGISTER_INTERFACE(ifile::IFilePersistenceInfo);
		I_ASSIGN_MULTI_0(m_slaveLoadersCompPtr, "SlaveLoaders", "List of slave loaders", true);
		I_ASSIGN(m_commonDescriptionAttrPtr, "CommonDescription", "Optional common description of this file type", false, "Composed");
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

	// reimplemented (ifile::IFilePersistenceInfo)
	virtual QString GetLastFilePath(OperationType operationType = OT_UNKNOWN, PathType pathType = PT_COMPLETE) const;

protected:
	QString GetPathForType(const QFileInfo& fileInfo, PathType pathType) const;

private:
	I_MULTIREF(ifile::IFilePersistence, m_slaveLoadersCompPtr);
	I_TEXTATTR(m_commonDescriptionAttrPtr);

	mutable QFileInfo m_lastOpenInfo;
	mutable QFileInfo m_lastSaveInfo;
};


} // namespace ifile


#endif // !ifile_CComposedFilePersistenceComp_included


