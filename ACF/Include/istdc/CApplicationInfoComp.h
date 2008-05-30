#ifndef istdc_CApplicationInfoComp_included
#define istdc_CApplicationInfoComp_included


#include "icomp/CComponentBase.h"

#include "ibase/IApplicationInfo.h"


namespace istdc
{


class CApplicationInfoComp: public icomp::CComponentBase, virtual public ibase::IApplicationInfo
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CApplicationInfoComp)
		I_REGISTER_INTERFACE(ibase::IApplicationInfo)
		I_REGISTER_INTERFACE(iser::IVersionInfo)
		I_ASSIGN_MULTI_0(m_userVersionsAttrPtr, "UserVersions", "List of user versions for specified user version ID's", false)
		I_ASSIGN_MULTI_0(m_userVersionIdsAttrPtr, "UserVersionIds", "List of user version ID's", false)
		I_ASSIGN_MULTI_0(m_userVersionIdDescriptionsAttrPtr, "UserVersionIdDescs", "Descriptions of user version ID's", false)
		I_ASSIGN(m_applicationNameAttrPtr, "ApplicationName", "Application name", true, "ACF Application")
		I_ASSIGN(m_companyNameAttrPtr, "CompanyName", "Company name", true, "ImagingTools")
	I_END_COMPONENT

	// reimplemented (ibase::IApplicationInfo)
	virtual istd::CString GetCompanyName() const;
	virtual istd::CString GetApplicationName() const;
	virtual istd::CString GetApplicationPath() const;
	virtual istd::CString EncodeVersionName(I_DWORD version, int versionId) const;

	// reimplemented (iser::IVersionInfo)
	virtual I_DWORD GetVersion(int versionId) const;
	virtual istd::CString GetVersionIdDescription(int versionId) const;
	virtual VersionIds GetVersionIds() const;

private:
	I_MULTIATTR(int, m_userVersionsAttrPtr);
	I_MULTIATTR(int, m_userVersionIdsAttrPtr);
	I_MULTIATTR(istd::CString, m_userVersionIdDescriptionsAttrPtr);
	I_ATTR(istd::CString, m_applicationNameAttrPtr);
	I_ATTR(istd::CString, m_companyNameAttrPtr);
};


} // namespace istdc


#endif // !istdc_CApplicationInfoComp_included


