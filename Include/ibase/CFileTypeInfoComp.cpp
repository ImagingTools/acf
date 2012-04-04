#include "ibase/CFileTypeInfoComp.h"


// Qt includes
#include <QtCore/QStringList>


namespace ibase
{


// reimplemented (iser::IFileTypeInfo)

bool CFileTypeInfoComp::GetFileExtensions(QStringList& result, int /*flags*/, bool doAppend) const
{
	if (m_fileExtensionsAttrPtr.IsValid()){
		if (!doAppend){
			result.clear();
		}

		int extensionsCount = m_fileExtensionsAttrPtr.GetCount();
		for (int i = 0; i < extensionsCount; ++i){
			const QString& extension = m_fileExtensionsAttrPtr[i];

			result.push_back(extension);
		}

		return true;
	}
	else{
		return false;
	}
}


QString CFileTypeInfoComp::GetTypeDescription(const QString* extensionPtr) const
{
	if (extensionPtr != NULL){
		int extensionsCount = qMin(m_fileExtensionsAttrPtr.GetCount(), m_typeDescriptionsAttrPtr.GetCount());
		for (int extIndex = 0; extIndex < extensionsCount; extIndex++){
			if (m_fileExtensionsAttrPtr[extIndex] == *extensionPtr){
				return m_typeDescriptionsAttrPtr[extIndex];
			}
		}
	}

	if (m_typeDescriptionsAttrPtr.GetCount() > 0){
		return m_typeDescriptionsAttrPtr[0];
	}

	return "";
}


} // namespace ibase


