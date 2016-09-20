#ifndef ibase_CVersionInfoComp_included
#define ibase_CVersionInfoComp_included


// ACF includes
#include <iser/IVersionInfo.h>
#include <icomp/CComponentBase.h>


namespace ibase
{


/**
	Component for providing of version information.
*/
class CVersionInfoComp: public icomp::CComponentBase, virtual public iser::IVersionInfo
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CVersionInfoComp);
		I_REGISTER_INTERFACE(iser::IVersionInfo);
		I_ASSIGN(m_versionIdAttrPtr, "VersionId", "Version ID", false, 1024);
		I_ASSIGN(m_versionNumberAttrPtr, "VersionNumber", "Current version number for VersionId", true, 0);
		I_ASSIGN(m_versionIdDescriptionAttrPtr, "VersionIdDesc", "Descriptions of version ID's", true, "User Version");
		I_ASSIGN_MULTI_0(m_knownVersionsAttrPtr, "KnownVersions", "Version numbers for known versions", false);
		I_ASSIGN_MULTI_0(m_knownVersionNamesAttrPtr, "KnownVersionNames", "Names of known versions", false);
		I_ASSIGN(m_isExtensionUsedAttrPtr, "IsExtensionUsed", "Enable using of automatic calculated minor version information at the end of encoded version string", true, true);
		I_ASSIGN(m_slaveVersionInfoCompPtr, "SlaveVersionInfo", "Slave version info, unknown version information requests will be delegated here", false, "SlaveVersionInfo");
	I_END_COMPONENT;

	// reimplemented (iser::IVersionInfo)
	virtual bool GetVersionNumber(int versionId, quint32& result) const;
	virtual QString GetVersionIdDescription(int versionId) const;
	virtual VersionIds GetVersionIds() const;
	virtual QString GetEncodedVersionName(int versionId, quint32 versionNumber) const;

private:
	I_ATTR(int, m_versionIdAttrPtr);
	I_ATTR(int, m_versionNumberAttrPtr);
	I_ATTR(QString, m_versionIdDescriptionAttrPtr);
	I_MULTIATTR(int, m_knownVersionsAttrPtr);
	I_MULTIATTR(QString, m_knownVersionNamesAttrPtr);
	I_ATTR(bool, m_isExtensionUsedAttrPtr);

	I_REF(iser::IVersionInfo, m_slaveVersionInfoCompPtr);
};


} // namespace ibase


#endif // !ibase_CVersionInfoComp_included


