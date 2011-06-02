#ifndef icmpstr_CVisualRegistryComp_included
#define icmpstr_CVisualRegistryComp_included


// Qt includes
#include <QObject>


// ACF includes
#include "icomp/IComponentEnvironmentManager.h"
#include "icomp/CRegistry.h"

#include "ibase/TLoggerCompWrap.h"

#include "icmpstr/CVisualRegistryElement.h"


namespace icmpstr
{


class CVisualRegistryComp:
			public QObject,
			public ibase::CLoggerComponentBase,
			public icomp::CRegistry
{
public:
	typedef ibase::CLoggerComponentBase BaseClass;
	typedef icomp::CRegistry BaseClass2;

	I_BEGIN_COMPONENT(CVisualRegistryComp);
		I_REGISTER_INTERFACE(istd::IChangeable);
		I_REGISTER_INTERFACE(icomp::IRegistry);
		I_ASSIGN(m_envManagerCompPtr, "MetaInfoManager", "Allows access to component meta information", true, "MetaInfoManager");
	I_END_COMPONENT;

	enum MessageId
	{
		MI_CANNOT_CREATE_ELEMENT = 650
	};

	virtual bool SerializeComponentsLayout(iser::IArchive& archive);
	virtual bool SerializeRegistry(iser::IArchive& archive);

	// reimplemented (icomp::IRegistry)
	virtual ElementInfo* InsertElementInfo(
				const std::string& elementId,
				const icomp::CComponentAddress& address,
				bool ensureElementCreated = true);
	virtual bool RenameElement(const std::string& oldElementId, const std::string& newElementId);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

protected:
	typedef imod::TModelWrap<istd::TChangeDelegator<CVisualRegistryElement> > Element;
	typedef istd::TDelPtr<QIcon> IconPtr;
	typedef std::map<icomp::CComponentAddress, IconPtr> IconMap;

	bool SerializeComponentPosition(iser::IArchive& archive, std::string& componentName, i2d::CVector2d& position);

	// reimplemented (icomp::CRegistry)
	virtual icomp::IRegistryElement* CreateRegistryElement(
				const std::string& elementId,
				const icomp::CComponentAddress& address) const;

private:
	I_REF(icomp::IComponentEnvironmentManager, m_envManagerCompPtr);
};


} // namespace icmpstr


#endif // !icmpstr_CVisualRegistryComp_included


