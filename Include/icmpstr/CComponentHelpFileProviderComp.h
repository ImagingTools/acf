#ifndef icmpstr_CComponentHelpFileProviderComp_included
#define icmpstr_CComponentHelpFileProviderComp_included


#include "icomp/IComponentStaticInfo.h"
#include "icomp/IRegistriesManager.h"
#include "icomp/IMetaInfoManager.h"
#include "icomp/CComponentBase.h"
#include "icomp/CComponentAddress.h"

#include "idoc/IHelpFileProvider.h"


namespace icmpstr
{


class CComponentHelpFileProviderComp:
			public icomp::CComponentBase,
			virtual public idoc::IHelpFileProvider
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CComponentHelpFileProviderComp);
		I_REGISTER_INTERFACE(idoc::IHelpFileProvider);
		I_ASSIGN(m_classHelpProviderCompPtr, "ClassHelpProvider", "Slave help file provider for class info objects", true, "ClassHelpProvider");
		I_ASSIGN(m_packagesLoaderInfoCompPtr, "PackagesLoader", "Packages loader used to get component info", true, "PackagesLoader");
		I_ASSIGN(m_metaInfoManagerCompPtr, "MetaInfoManager", "Allows access to component meta information", true, "MetaInfoManager");
	I_END_COMPONENT;

	// reimplemented (idoc::IHelpFileProvider)
	virtual double GetHelpQuality(const istd::CString& contextText, const istd::IPolymorphic* contextObjectPtr) const;
	virtual istd::CString GetHelpFilePath(const istd::CString& contextText, const istd::IPolymorphic* contextObjectPtr) const;

protected:
	virtual istd::CString GetInfoFilePath(const icomp::CComponentAddress& componentAddress) const;
	virtual istd::CString GetSlaveFilePath(const icomp::CComponentAddress& componentAddress) const;

	bool ExtractComponentAddress(const istd::CString& contextText, const istd::IPolymorphic* contextObjectPtr, icomp::CComponentAddress& result) const;

private:
	I_REF(idoc::IHelpFileProvider, m_classHelpProviderCompPtr);
	I_REF(icomp::IComponentStaticInfo, m_packagesLoaderInfoCompPtr);
	I_REF(icomp::IMetaInfoManager, m_metaInfoManagerCompPtr);
};


} // namespace icmpstr


#endif // !icmpstr_CComponentHelpFileProviderComp_included


