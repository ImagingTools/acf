#include "ibase/CApplicationInfoComp.h"


#include "istd/Generated/AcfVersion.h"


namespace ibase
{


// public methods

// reimplemented (ibase::IApplicationInfo)

istd::CString CApplicationInfoComp::GetCompanyName() const
{
	if (m_companyNameAttrPtr.IsValid()){
		return *m_companyNameAttrPtr;
	}
	else if (m_slaveVersionInfoCompPtr.IsValid()){
		return m_slaveVersionInfoCompPtr->GetCompanyName();
	}
	else{
		return "";
	}
}


istd::CString CApplicationInfoComp::GetApplicationName() const
{
	if (m_applicationNameAttrPtr.IsValid()){
		return *m_applicationNameAttrPtr;
	}
	else if (m_slaveVersionInfoCompPtr.IsValid()){
		return m_slaveVersionInfoCompPtr->GetApplicationName();
	}
	else{
		return "";
	}
}


istd::CString CApplicationInfoComp::GetApplicationPath() const
{
	return istd::CString(".");
}


istd::CString CApplicationInfoComp::GetEncodedVersionName(int versionId, I_DWORD versionNumber) const
{
	istd::CString retVal = "0";

	if (m_versionIdAttrPtr.IsValid() && (versionId == *m_versionIdAttrPtr)){
		I_DWORD lastBellowNumber = 0;
		int knownVersionsCount = istd::Min(m_knownVersionsAttrPtr.GetCount(), m_knownVersionNamesAttrPtr.GetCount());
		for (int i = 0; i < knownVersionsCount; ++i){
			I_DWORD knownNumber = I_DWORD(m_knownVersionsAttrPtr[i]);

			if ((knownNumber <= versionNumber) && (knownNumber > lastBellowNumber)){
				lastBellowNumber = knownNumber;
				retVal = m_knownVersionNamesAttrPtr[i];
			}
		}

		if (m_isExtensionUsedAttrPtr.IsValid() && *m_isExtensionUsedAttrPtr){
			retVal += istd::CString(".") + istd::CString::FromNumber(int(versionNumber - lastBellowNumber));
		}
	}
	else if (m_slaveVersionInfoCompPtr.IsValid()){
		retVal = m_slaveVersionInfoCompPtr->GetEncodedVersionName(versionId, versionNumber);
	}
	else{
		retVal = istd::CString("<") + istd::CString::FromNumber(versionNumber) + ">";
	}

	return retVal;
}


// reimplemented (iser::IVersionInfo)

bool CApplicationInfoComp::GetVersionNumber(int versionId, I_DWORD& result) const
{
	if (m_versionIdAttrPtr.IsValid() && (versionId == *m_versionIdAttrPtr)){
		result = *m_versionNumberAttrPtr;

		return true;
	}
	else if (m_slaveVersionInfoCompPtr.IsValid()){
		return m_slaveVersionInfoCompPtr->GetVersionNumber(versionId, result);
	}
	else{
		if (versionId == iser::IVersionInfo::FrameworkVersionId){
			result = istd::RS_USE_VERSION;

			return true;
		}

		return false;
	}
}


istd::CString CApplicationInfoComp::GetVersionIdDescription(int versionId) const
{
	if (m_versionIdAttrPtr.IsValid() && (versionId == *m_versionIdAttrPtr)){
		return *m_versionIdDescriptionAttrPtr;
	}
	else if (m_slaveVersionInfoCompPtr.IsValid()){
		return m_slaveVersionInfoCompPtr->GetVersionIdDescription(versionId);
	}
	else{
		return "";
	}
}


iser::IVersionInfo::VersionIds CApplicationInfoComp::GetVersionIds() const
{
	VersionIds retVal;
	if (m_slaveVersionInfoCompPtr.IsValid()){
		retVal = m_slaveVersionInfoCompPtr->GetVersionIds();
	}

	if (m_versionIdAttrPtr.IsValid()){
		retVal.insert(*m_versionIdAttrPtr);
	}

	return retVal;
}


} // namespace ibase


