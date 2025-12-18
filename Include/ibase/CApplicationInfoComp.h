#pragma once


// ACF includes
#include <iser/IVersionInfo.h>
#include <icomp/CComponentBase.h>
#include <ibase/IApplicationInfo.h>


namespace ibase
{


/**
	Component for providing of application info such as program name, type, copyright text etc.
*/
class CApplicationInfoComp:
			public icomp::CComponentBase,
			virtual public ibase::IApplicationInfo,
			virtual protected iser::IVersionInfo
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CApplicationInfoComp);
		I_REGISTER_INTERFACE(ibase::IApplicationInfo);
		I_ASSIGN(m_companyNameAttrPtr, "CompanyName", "Company name", false, "<my company>");
		I_ASSIGN(m_productNameAttrPtr, "ProductName", "Product name, the same for all applications in one product", false, "<my product>");
		I_ASSIGN(m_productEditionAttrPtr, "ProductEdition", "Product edition", false, "");
		I_ASSIGN(m_applicationIdAttrPtr, "ApplicationId", "Application-ID", true, "ApplicationId");
		I_ASSIGN(m_applicationNameAttrPtr, "ApplicationName", "Application name", false, "ACF Application");
		I_ASSIGN(m_applicationSubnameAttrPtr, "ApplicationSubname", "Sub-name of application", false, "");
		I_ASSIGN(m_applicationTypeAttrPtr, "ApplicationType", "Application type", false, "Server");
		I_ASSIGN(m_legalCopyrightAttrPtr, "LegalCopyright", "Legal copyright of this application", false, "\x00A9 xxxx <my company>");
		I_ASSIGN(m_mainVersionIdAttrPtr, "MainVersionId", "ID of main module version as defined in version info", false, 0);
		I_ASSIGN(m_versionInfoCompPtr, "VersionInfo", "Provide information about global application version and version of related packages", true, "VersionInfo");
	I_END_COMPONENT;

	// reimplemented (ibase::IApplicationInfo)
	virtual QString GetApplicationAttribute(int attributeId, bool allowTranslation = true) const override;
	virtual int GetMainVersionId() const override;
	virtual const iser::IVersionInfo& GetVersionInfo() const override;

protected:
	// reimplemented (iser::IVersionInfo)
	virtual bool GetVersionNumber(int versionId, quint32& result) const override;
	virtual QString GetVersionIdDescription(int versionId) const override;
	virtual VersionIds GetVersionIds() const override;
	virtual QString GetEncodedVersionName(int versionId, quint32 versionNumber) const override;

private:
	I_TEXTATTR(m_companyNameAttrPtr);
	I_TEXTATTR(m_productNameAttrPtr);
	I_TEXTATTR(m_productEditionAttrPtr);
	I_ATTR(QByteArray, m_applicationIdAttrPtr);
	I_TEXTATTR(m_applicationNameAttrPtr);
	I_TEXTATTR(m_applicationSubnameAttrPtr);
	I_TEXTATTR(m_applicationTypeAttrPtr);
	I_TEXTATTR(m_legalCopyrightAttrPtr);
	I_ATTR(int, m_mainVersionIdAttrPtr);
	I_REF(iser::IVersionInfo, m_versionInfoCompPtr);
};


} // namespace ibase


