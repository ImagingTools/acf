#ifndef icmpstr_CVisualRegistryComp_included
#define icmpstr_CVisualRegistryComp_included


#include "icomp/CRegistry.h"
#include "icomp/CPackageStaticInfo.h"
#include "icomp/CComponentBase.h"

#include "iser/IObject.h"
#include "ibase/TLoggerCompWrap.h"
#include "ibase/TFactorisableContainer.h"

#include "icmpstr/IRegistryEditController.h"
#include "icmpstr/CVisualRegistryElement.h"


namespace icmpstr
{


class CVisualRegistryComp:
			public ibase::CLoggerComponentBase,
			public icomp::CRegistry,
			public ibase::TFactorisableContainer<iser::IObject>
{
public:
	typedef ibase::CLoggerComponentBase BaseClass;
	typedef icomp::CRegistry BaseClass2;

	I_BEGIN_COMPONENT(CVisualRegistryComp);
		I_REGISTER_INTERFACE(istd::IChangeable);
		I_REGISTER_INTERFACE(icomp::IRegistry);
		I_ASSIGN(m_staticInfoCompPtr, "StaticComponentInfo", "Static Component Info", true, "StaticComponentInfo");
	I_END_COMPONENT;

	enum ChangeFlags
	{
		CF_NOTE = 0x40000
	};

	enum MessageId
	{
		MI_CANNOT_CREATE_ELEMENT = 650
	};

	virtual bool SerializeComponentsLayout(iser::IArchive& archive);
	virtual bool SerializeRegistry(iser::IArchive& archive);

	enum ConsistencyState
	{
		CS_UNKNOWN,
		CS_OK,
		CS_OPTIONAL,
		CS_INVALID
	};
	int CheckAttributeConsistency(const icomp::IRegistryElement& element, const std::string& attributeId);

	// reimplemented (icomp::IComponent)
	virtual void OnComponentCreated();

	// reimplemented (icomp::IRegistry)
	virtual ElementInfo* InsertElementInfo(
				const std::string& elementId,
				const icomp::CComponentAddress& address,
				bool ensureElementCreated = true);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

protected:
	typedef imod::TModelWrap<istd::TChangeDelegator<CVisualRegistryElement> > Element;

	bool SerializeComponentPosition(iser::IArchive& archive, std::string& componentName, i2d::CVector2d& position);

	// reimplemented (icomp::CRegistry)
	virtual icomp::IRegistryElement* CreateRegistryElement(const icomp::CComponentAddress& address) const;

private:
	I_REF(icomp::IComponentStaticInfo, m_staticInfoCompPtr);
};


} // namespace icmpstr


#endif // !icmpstr_CVisualRegistryComp_included

