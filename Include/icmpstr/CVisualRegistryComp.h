#ifndef icmpstr_CVisualRegistryComp_included
#define icmpstr_CVisualRegistryComp_included


#include "iser/IObject.h"

#include "icomp/IComponentEnvironmentManager.h"
#include "icomp/CRegistry.h"
#include "icomp/CPackageStaticInfo.h"
#include "icomp/CComponentBase.h"

#include "ibase/TLoggerCompWrap.h"

#include "icmpstr/IElementSelectionInfo.h"
#include "icmpstr/CVisualRegistryElement.h"


namespace icmpstr
{


class CVisualRegistryComp:
			public ibase::CLoggerComponentBase,
			public icomp::CRegistry,
			public icmpstr::IElementSelectionInfo
{
public:
	typedef ibase::CLoggerComponentBase BaseClass;
	typedef icomp::CRegistry BaseClass2;

	I_BEGIN_COMPONENT(CVisualRegistryComp);
		I_REGISTER_INTERFACE(istd::IChangeable);
		I_REGISTER_INTERFACE(icomp::IRegistry);
		I_REGISTER_INTERFACE(icmpstr::IElementSelectionInfo);
		I_ASSIGN(m_envManagerCompPtr, "MetaInfoManager", "Allows access to component meta information", true, "MetaInfoManager");
	I_END_COMPONENT;

	enum ChangeFlags
	{
		CF_NOTE = 0x40000,
		CF_SELECTION = 0x80000
	};

	enum MessageId
	{
		MI_CANNOT_CREATE_ELEMENT = 650
	};

	virtual bool SerializeComponentsLayout(iser::IArchive& archive);
	virtual bool SerializeRegistry(iser::IArchive& archive);
	virtual void SetSelectedElement(CVisualRegistryElement* selectedElementPtr);

	// reimplemented (icmpstr::IElementSelectionInfo)
	virtual icomp::IRegistry* GetSelectedRegistry() const;
	virtual iser::ISerializable* GetSelectedElement() const;
	virtual const std::string& GetSelectedElementName() const;
	virtual const icomp::CComponentAddress* GetSelectedElementAddress() const;

	// reimplemented (icomp::IRegistry)
	virtual ElementInfo* InsertElementInfo(
				const std::string& elementId,
				const icomp::CComponentAddress& address,
				bool ensureElementCreated = true);
	virtual bool RemoveElementInfo(const std::string& elementId);
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

	std::string m_selectedElementId;
};


} // namespace icmpstr


#endif // !icmpstr_CVisualRegistryComp_included

