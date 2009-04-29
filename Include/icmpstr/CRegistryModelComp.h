#ifndef CRegistryModelComp_included
#define CRegistryModelComp_included


#include "icomp/CRegistry.h"
#include "icomp/CPackageStaticInfo.h"
#include "icomp/CComponentBase.h"

#include "ibase/TLoggerCompWrap.h"

#include "icmpstr/IRegistryEditController.h"


namespace icmpstr
{


class CRegistryModelComp:
			public ibase::TLoggerCompWrap<icomp::CComponentBase>,
			public icomp::CRegistry,
			virtual public IRegistryEditController
{
public:
	typedef ibase::TLoggerCompWrap<icomp::CComponentBase> BaseClass;
	typedef icomp::CRegistry BaseClass2;

	I_BEGIN_COMPONENT(CRegistryModelComp)
		I_REGISTER_INTERFACE(icomp::IRegistry)
		I_ASSIGN(m_staticInfoCompPtr, "StaticComponentInfo", "Static Component Info", true, "StaticComponentInfo")
	I_END_COMPONENT

	enum ChangeFlags
	{
		CF_POSITION = 0x20000,
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

	// reimplemented (icmpstr::IRegistryEditController)
	virtual i2d::CVector2d GetComponentPosition(const std::string& componentName) const;
	virtual void SetComponentPosition(const std::string& componentName, const i2d::CVector2d& point);
	virtual istd::CString GetComponentNote(const std::string& componentName) const;
	virtual void SetComponentNote(const std::string& componentName, const istd::CString& componentNote);

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
	bool SerializeComponentPosition(iser::IArchive& archive, std::string& componentName, i2d::CVector2d& position);
	bool SerializeNote(iser::IArchive& archive, std::string& componentName, istd::CString& note);

private:
	I_REF(icomp::IComponentStaticInfo, m_staticInfoCompPtr);

	typedef std::map<std::string, i2d::CVector2d> ElementsPositionMap;
	typedef std::map<std::string, istd::CString> ElementsNoteMap;

	ElementsPositionMap m_elementsPositionMap;
	ElementsNoteMap m_elementsNoteMap;
};


} // namespace icmpstr


#endif // !CRegistryModelComp_included

