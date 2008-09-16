#include "ibase/CApplicationInfoComp.h"


namespace ibase
{


// public methods

// reimplemented (ibase::IApplicationInfo)

istd::CString CApplicationInfoComp::GetCompanyName() const
{
	I_ASSERT(m_companyNameAttrPtr.IsValid());	// This attribute is obligatory

	return *m_companyNameAttrPtr;
}


istd::CString CApplicationInfoComp::GetApplicationName() const
{
	I_ASSERT(m_applicationNameAttrPtr.IsValid());	// This attribute is obligatory

	return *m_applicationNameAttrPtr;
}


istd::CString CApplicationInfoComp::GetApplicationPath() const
{
	return istd::CString(".");
}


istd::CString CApplicationInfoComp::EncodeVersionName(I_DWORD version, int /*versionId*/) const
{
	return istd::CString("(") + istd::CString::FromNumber(version) + ")";
}


// reimplemented (iser::IVersionInfo)

I_DWORD CApplicationInfoComp::GetVersion(int versionId) const
{
	int idsCount = m_userVersionIdsAttrPtr.GetCount();
	for (int i = 0; i < idsCount; ++i){
		if (m_userVersionIdsAttrPtr[i] == versionId){
			if (i < m_userVersionsAttrPtr.GetCount()){
				return m_userVersionsAttrPtr[i];
			}

			return 0;
		}
	}

	return 0;
}


istd::CString CApplicationInfoComp::GetVersionIdDescription(int versionId) const
{
	if (versionId < m_userVersionIdDescriptionsAttrPtr.GetCount()){
		return m_userVersionIdDescriptionsAttrPtr[versionId];
	}
	else{
		return istd::CString("<User") + istd::CString::FromNumber(versionId) + istd::CString(">");
	}
}


iser::IVersionInfo::VersionIds CApplicationInfoComp::GetVersionIds() const
{
	VersionIds retVal;

	int idsCount = m_userVersionIdsAttrPtr.GetCount();
	for (int i = 0; i < idsCount; ++i){
		retVal.insert(m_userVersionIdsAttrPtr[i]);
	}

	return retVal;
}


} // namespace ibase


