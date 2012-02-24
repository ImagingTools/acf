#include "ibase/CApplicationInfoComp.h"


namespace ibase
{


// public methods

// reimplemented (ibase::IApplicationInfo)

QString CApplicationInfoComp::GetApplicationAttribute(int attributeId) const
{
	switch (attributeId){
	case AA_COMPANY_NAME:
		if (m_companyNameAttrPtr.IsValid()){
			return *m_companyNameAttrPtr;
		}
		break;

	case AA_PRODUCT_NAME:
		if (m_productNameAttrPtr.IsValid()){
			return *m_productNameAttrPtr;
		}
		break;

	case AA_APPLICATION_NAME:
		if (m_applicationNameAttrPtr.IsValid()){
			return *m_applicationNameAttrPtr;
		}
		break;

	case AA_APPLICATION_SUBNAME:
		if (m_applicationSubnameAttrPtr.IsValid()){
			return *m_applicationSubnameAttrPtr;
		}
		break;

	case AA_APPLICATION_TYPE:
		if (m_applicationTypeAttrPtr.IsValid()){
			return *m_applicationTypeAttrPtr;
		}
		break;

	case AA_APPLICATION_PATH:
		return QString(".");
		break;

	case AA_LEGAL_COPYRIGHT:
		if (m_legalCopyrightAttrPtr.IsValid()){
			return *m_legalCopyrightAttrPtr;
		}
		break;

	case AA_MAIN_VERSION:
		if (m_mainVersionIdAttrPtr.IsValid() && m_versionInfoCompPtr.IsValid()){
			I_DWORD versionNumber;
			if (m_versionInfoCompPtr->GetVersionNumber(*m_mainVersionIdAttrPtr, versionNumber)){
				return m_versionInfoCompPtr->GetEncodedVersionName(*m_mainVersionIdAttrPtr, versionNumber);
			}
		}
		break;

	default:
		break;
	}

	return "";
}


int CApplicationInfoComp::GetMainVersionId() const
{
	int retVal = -1;

	if (m_mainVersionIdAttrPtr.IsValid()){
		retVal = *m_mainVersionIdAttrPtr;
	}

	return retVal;
}


const iser::IVersionInfo& CApplicationInfoComp::GetVersionInfo() const
{
	if (m_versionInfoCompPtr.IsValid()){
		return *m_versionInfoCompPtr;
	}

	return *this;	// empty version info
}


// public methods

// reimplemented (iser::IVersionInfo)

bool CApplicationInfoComp::GetVersionNumber(int /*versionId*/, I_DWORD& result) const
{
	result = 0xffffffff;

	return false;
}


QString CApplicationInfoComp::GetVersionIdDescription(int /*versionId*/) const
{
	return "";
}


iser::IVersionInfo::VersionIds CApplicationInfoComp::GetVersionIds() const
{
	return iser::IVersionInfo::VersionIds();
}


QString CApplicationInfoComp::GetEncodedVersionName(int /*versionId*/, I_DWORD /*versionNumber*/) const
{
	return "";
}


} // namespace ibase


