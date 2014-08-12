#ifndef ifile_CFileTypeInfoComp_included
#define ifile_CFileTypeInfoComp_included


// ACF includes
#include "icomp/CComponentBase.h"
#include "ifile/IFileTypeInfo.h"


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

	enum MessageId
	{
		MI_BAD_EXTENSION = 0xac10,
		MI_UNSUPPORTED_VERSION
	};

	I_BEGIN_COMPONENT(CFileTypeInfoComp);
		I_REGISTER_INTERFACE(ifile::IFileTypeInfo);
		I_ASSIGN_MULTI_0(m_fileExtensionsAttrPtr, "FileExtensions", "List of supported file extensions", false);
		I_ASSIGN_MULTI_0(m_typeDescriptionsAttrPtr, "TypeDescriptions", "List of descriptions for each extension", false);
	I_END_COMPONENT;

	// reimplemented (ifile::IFileTypeInfo)
	virtual bool GetFileExtensions(QStringList& result, const istd::IChangeable* dataObjectPtr = NULL, int flags = -1, bool doAppend = false) const;
	virtual QString GetTypeDescription(const QString* extensionPtr = NULL) const;

private:
	I_MULTIATTR(QString, m_fileExtensionsAttrPtr);
	I_MULTIATTR(QString, m_typeDescriptionsAttrPtr);
};


} // namespace ifile


#endif // !ifile_CFileTypeInfoComp_included


