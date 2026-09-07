// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <ibase/CApplicationInfoComp.h>


// Qt includes
#include <QtCore/QCoreApplication>


namespace ibase
{


// public methods

// reimplemented (ibase::IApplicationInfo)

QString CApplicationInfoComp::GetApplicationAttribute(int attributeId, bool /*allowTranslation*/) const
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

	case AA_PRODUCT_EDITION:
		if (m_productEditionAttrPtr.IsValid()){
			return *m_productEditionAttrPtr;
		}
		break;

	case AA_APPLICATION_ID:
		if (m_applicationIdAttrPtr.IsValid()){
			return *m_applicationIdAttrPtr;
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
			quint32 versionNumber;
			if (m_versionInfoCompPtr->GetVersionNumber(*m_mainVersionIdAttrPtr, versionNumber)){
				return m_versionInfoCompPtr->GetEncodedVersionName(*m_mainVersionIdAttrPtr, versionNumber);
			}
		}
		break;

	case AA_VERSION_KIND:
		switch (GetVersionKind()){
		case VK_BETA:
			return allowTranslation?
						QCoreApplication::translate("ibase::IApplicationInfo", "Beta Version"):
						QString("Beta Version");

		case VK_RELEASE_CANDIDATE:
			return allowTranslation?
						QCoreApplication::translate("ibase::IApplicationInfo", "Release Candidate"):
						QString("Release Candidate");

		case VK_RELEASE:
			return allowTranslation?
						QCoreApplication::translate("ibase::IApplicationInfo", "Release"):
						QString("Release");

		case VK_DEVELOPER:
		default:
			return allowTranslation?
						QCoreApplication::translate("ibase::IApplicationInfo", "Developer Version"):
						QString("Developer Version");
		}
		break;

	default:
		break;
	}

	return QString();
}


IApplicationInfo::VersionKind CApplicationInfoComp::GetVersionKind() const
{
#if defined(ACF_VERSION_KIND_RELEASE)
	return VK_RELEASE;
#elif defined(ACF_VERSION_KIND_RELEASE_CANDIDATE)
	return VK_RELEASE_CANDIDATE;
#elif defined(ACF_VERSION_KIND_BETA)
	return VK_BETA;
#else
	// no version kind was selected via build pipeline (e.g. build on a developer PC)
	return VK_DEVELOPER;
#endif
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

bool CApplicationInfoComp::GetVersionNumber(int /*versionId*/, quint32& result) const
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


QString CApplicationInfoComp::GetEncodedVersionName(int /*versionId*/, quint32 /*versionNumber*/) const
{
	return "";
}


} // namespace ibase


