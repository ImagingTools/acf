#ifndef ibase_CFileTypeInfoComp_included
#define ibase_CFileTypeInfoComp_included


#include "iser/IFileTypeInfo.h"

#include "icomp/CComponentBase.h"


namespace ibase
{


/**
	Simple implementation of iser::IFileTypeInfo using user defined extension list and descriptions.
*/
class CFileTypeInfoComp:
			public icomp::CComponentBase,
			virtual public iser::IFileTypeInfo
{
public:
	typedef icomp::CComponentBase BaseClass;

	enum MessageId
	{
		MI_BAD_EXTENSION = 0xac10,
		MI_UNSUPPORTED_VERSION
	};

	I_BEGIN_COMPONENT(CFileTypeInfoComp);
		I_REGISTER_INTERFACE(iser::IFileTypeInfo);
		I_ASSIGN_MULTI_0(m_fileExtensionsAttrPtr, "FileExtensions", "List of supported file extensions", false);
		I_ASSIGN_MULTI_0(m_typeDescriptionsAttrPtr, "TypeDescriptions", "List of descriptions for each extension", false);
	I_END_COMPONENT;

	// reimplemented (iser::IFileTypeInfo)
	virtual bool GetFileExtensions(QStringList& result, int flags = -1, bool doAppend = false) const;
	virtual QString GetTypeDescription(const QString* extensionPtr = NULL) const;

private:
	I_MULTIATTR(QString, m_fileExtensionsAttrPtr);
	I_MULTIATTR(QString, m_typeDescriptionsAttrPtr);
};


} // namespace ibase


#endif // !ibase_CFileTypeInfoComp_included


