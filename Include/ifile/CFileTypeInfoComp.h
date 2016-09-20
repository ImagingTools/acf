#ifndef ifile_CFileTypeInfoComp_included
#define ifile_CFileTypeInfoComp_included


// ACF includes
#include <icomp/CComponentBase.h>
#include <ifile/IFileTypeInfo.h>


namespace ifile
{


/**
	Simple implementation of ifile::IFileTypeInfo using user defined extension list and descriptions.

	\ingroup Persistence
*/
class CFileTypeInfoComp:
			public icomp::CComponentBase,
			virtual public ifile::IFileTypeInfo
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CFileTypeInfoComp);
		I_REGISTER_INTERFACE(ifile::IFileTypeInfo);
		I_ASSIGN_MULTI_0(m_fileExtensionsAttrPtr, "FileExtensions", "List of supported file extensions", false);
		I_ASSIGN_MULTI_0(m_typeDescriptionsAttrPtr, "TypeDescriptions", "List of descriptions for each extension", false);
		I_ASSIGN_MULTI_0(m_fileExtensionsWinAttrPtr, "FileExtensionsWin", "List of supported file extensions for Windows systems only", false);
		I_ASSIGN_MULTI_0(m_typeDescriptionsWinAttrPtr, "TypeDescriptionsWin", "List of descriptions for each extension for Windows systems only", false);
		I_ASSIGN_MULTI_0(m_fileExtensionsMacAttrPtr, "FileExtensionsMac", "List of supported file extensions for MAC systems only", false);
		I_ASSIGN_MULTI_0(m_typeDescriptionsMacAttrPtr, "TypeDescriptionsMac", "List of descriptions for each extension for MAC systems only", false);
		I_ASSIGN_MULTI_0(m_fileExtensionsUnxAttrPtr, "FileExtensionsUnx", "List of supported file extensions for Unix-type systems only (without MAC)", false);
		I_ASSIGN_MULTI_0(m_typeDescriptionsUnxAttrPtr, "TypeDescriptionsUnx", "List of descriptions for each extension for Unix-type systems only (without MAC)", false);
	I_END_COMPONENT;

	// reimplemented (ifile::IFileTypeInfo)
	virtual bool GetFileExtensions(QStringList& result, const istd::IChangeable* dataObjectPtr = NULL, int flags = -1, bool doAppend = false) const;
	virtual QString GetTypeDescription(const QString* extensionPtr = NULL) const;

private:
	I_MULTIATTR(QString, m_fileExtensionsAttrPtr);
	I_MULTIATTR(QString, m_typeDescriptionsAttrPtr);
	I_MULTIATTR(QString, m_fileExtensionsWinAttrPtr);
	I_MULTIATTR(QString, m_typeDescriptionsWinAttrPtr);
	I_MULTIATTR(QString, m_fileExtensionsMacAttrPtr);
	I_MULTIATTR(QString, m_typeDescriptionsMacAttrPtr);
	I_MULTIATTR(QString, m_fileExtensionsUnxAttrPtr);
	I_MULTIATTR(QString, m_typeDescriptionsUnxAttrPtr);
};


} // namespace ifile


#endif // !ifile_CFileTypeInfoComp_included


