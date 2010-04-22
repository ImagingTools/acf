#ifndef ibase_CApplicationInfoComp_included
#define ibase_CApplicationInfoComp_included


#include "icomp/CComponentBase.h"

#include "ibase/IApplicationInfo.h"


namespace ibase
{


/**
	Component for providing of application info such as program version, creator, owner etc.
*/
class CApplicationInfoComp: public icomp::CComponentBase, virtual public ibase::IApplicationInfo
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CApplicationInfoComp)
		I_REGISTER_INTERFACE(ibase::IApplicationInfo);
		I_REGISTER_INTERFACE(iser::IVersionInfo);
		I_ASSIGN(m_versionIdAttrPtr, "VersionId", "Version ID", false, 1024);
		I_ASSIGN(m_versionNumberAttrPtr, "VersionNumber", "Current version number for VersionId", true, 0);
		I_ASSIGN(m_versionIdDescriptionAttrPtr, "VersionIdDesc", "Descriptions of version ID's", true, "User Version");
		I_ASSIGN(m_applicationNameAttrPtr, "ApplicationName", "Application name", false, "ACF Application");
		I_ASSIGN(m_companyNameAttrPtr, "CompanyName", "Company name", false, "ImagingTools");
		I_ASSIGN_MULTI_0(m_knownVersionsAttrPtr, "KnownVersions", "Version numbers for known versions", false);
		I_ASSIGN_MULTI_0(m_knownVersionNamesAttrPtr, "KnownVersionNames", "Names of known versions", false);
		I_ASSIGN(m_isExtensionUsedAttrPtr, "IsExtensionUsed", "Enable using of automatic calculated minor version information at the end of encoded version string", true, true);
		I_ASSIGN(m_slaveVersionInfoCompPtr, "SlaveApplicationInfo", "Slave application and version info used to get undefinded infos", false, "SlaveApplicationInfo");
	I_END_COMPONENT

	// reimplemented (ibase::IApplicationInfo)
	virtual istd::CString GetCompanyName() const;
	virtual istd::CString GetApplicationName() const;
	virtual istd::CString GetApplicationPath() const;
	virtual istd::CString GetEncodedVersionName(int versionId, I_DWORD versionNumber) const;

	// reimplemented (iser::IVersionInfo)
	virtual bool GetVersionNumber(int versionId, I_DWORD& result) const;
	virtual istd::CString GetVersionIdDescription(int versionId) const;
	virtual VersionIds GetVersionIds() const;

private:
	I_ATTR(int, m_versionIdAttrPtr);
	I_ATTR(int, m_versionNumberAttrPtr);
	I_ATTR(istd::CString, m_versionIdDescriptionAttrPtr);
	I_ATTR(istd::CString, m_applicationNameAttrPtr);
	I_ATTR(istd::CString, m_companyNameAttrPtr);
	I_MULTIATTR(int, m_knownVersionsAttrPtr);
	I_MULTIATTR(istd::CString, m_knownVersionNamesAttrPtr);
	I_ATTR(bool, m_isExtensionUsedAttrPtr);
	I_REF(ibase::IApplicationInfo, m_slaveVersionInfoCompPtr);
};


} // namespace ibase


#endif // !ibase_CApplicationInfoComp_included


