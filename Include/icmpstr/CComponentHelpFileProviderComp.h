#ifndef icmpstr_CComponentHelpFileProviderComp_included
#define icmpstr_CComponentHelpFileProviderComp_included


#include "icomp/IComponentStaticInfo.h"
#include "icomp/IRegistriesManager.h"
#include "icomp/IMetaInfoManager.h"
#include "icomp/CComponentBase.h"
#include "icomp/CComponentAddress.h"

#include "idoc/IHelpFileProvider.h"

#include "icmpstr/IExternalMetaInfoManager.h"


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
		I_ASSIGN(m_slaveProviderCompPtr, "SlaveProvider", "Slave help file provider used if no class description is found", false, "SlaveProvider");
		I_ASSIGN(m_metaInfoManagerCompPtr, "MetaInfoManager", "Allows access to component meta information", true, "MetaInfoManager");
		I_ASSIGN_TO(m_externalMetaInfoManagerCompPtr, m_metaInfoManagerCompPtr, false);
		I_ASSIGN(m_helpFileNameAttrPtr, "HelpFileName", "Name of help file in component info directory", true, "ShortDescription.html");
	I_END_COMPONENT;

	// reimplemented (idoc::IHelpFileProvider)
	virtual double GetHelpQuality(const QString& contextText, const istd::IPolymorphic* contextObjectPtr) const;
	virtual QString GetHelpFilePath(const QString& contextText, const istd::IPolymorphic* contextObjectPtr) const;

protected:
	virtual QString GetInfoFilePath(const icomp::CComponentAddress& componentAddress) const;
	virtual QString GetHelpFilePath(const icomp::CComponentAddress& componentAddress) const;

	bool ExtractComponentAddress(const QString& contextText, const istd::IPolymorphic* contextObjectPtr, icomp::CComponentAddress& result) const;

private:
	I_REF(idoc::IHelpFileProvider, m_slaveProviderCompPtr);
	I_REF(icomp::IMetaInfoManager, m_metaInfoManagerCompPtr);
	I_REF(IExternalMetaInfoManager, m_externalMetaInfoManagerCompPtr);
	I_ATTR(QString, m_helpFileNameAttrPtr);
};


} // namespace icmpstr


#endif // !icmpstr_CComponentHelpFileProviderComp_included


