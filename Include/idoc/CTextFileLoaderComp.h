#ifndef idoc_CTextFileLoaderComp_included
#define idoc_CTextFileLoaderComp_included


// ACF includes
#include <ifile/IFilePersistence.h>
#include <icomp/CComponentBase.h>


namespace idoc
{


/**
	Persistence component for the files contains plain text.
*/
class CTextFileLoaderComp: public icomp::CComponentBase, public ifile::IFilePersistence
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CTextFileLoaderComp);
		I_REGISTER_INTERFACE(ifile::IFileTypeInfo);
		I_REGISTER_INTERFACE(ifile::IFilePersistence);
		I_ASSIGN_MULTI_1(m_fileExtensionsAttrPtr, "FileExtensions", "List of possible file extensions", true, "txt");
		I_ASSIGN_MULTI_1(m_typeDescriptionsAttrPtr, "TypeDescriptions", "List of descriptions for each extension", true, "Text file");
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

private:
	I_MULTIATTR(QString, m_fileExtensionsAttrPtr);
	I_MULTITEXTATTR(m_typeDescriptionsAttrPtr);
};


} // namespace idoc


#endif // !idoc_CTextFileLoaderComp_included


