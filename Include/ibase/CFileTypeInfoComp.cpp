#include "ibase/CFileTypeInfoComp.h"


namespace ibase
{


// reimplemented (iser::IFileTypeInfo)

bool CFileTypeInfoComp::GetFileExtensions(istd::CStringList& result, int /*flags*/, bool doAppend) const
{
	if (m_fileExtensionsAttrPtr.IsValid()){
		if (!doAppend){
			result.clear();
		}

		int extensionsCount = m_fileExtensionsAttrPtr.GetCount();
		for (int i = 0; i < extensionsCount; ++i){
			const istd::CString& extension = m_fileExtensionsAttrPtr[i];

			result.push_back(extension);
		}

		return true;
	}
	else{
		return false;
	}
}


istd::CString CFileTypeInfoComp::GetTypeDescription(const istd::CString* extensionPtr) const
{
	if (extensionPtr != NULL){
		int extensionsCount = istd::Min(m_fileExtensionsAttrPtr.GetCount(), m_typeDescriptionsAttrPtr.GetCount());
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


