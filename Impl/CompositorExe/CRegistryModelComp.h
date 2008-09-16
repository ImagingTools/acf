#ifndef CRegistryModelComp_included
#define CRegistryModelComp_included


#include "icomp/IRegistryGeometryProvider.h"
#include "icomp/IRegistryNotesProvider.h"
#include "icomp/CRegistry.h"
#include "icomp/CPackageStaticInfo.h"
#include "icomp/CComponentBase.h"


class CRegistryModelComp:	public icomp::CComponentBase,
							public icomp::CRegistry,
							virtual public icomp::IRegistryGeometryProvider,
							virtual public icomp::IRegistryNotesProvider
{
public:
	typedef icomp::CComponentBase BaseClass;
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

	virtual bool SerializeComponentsLayout(iser::IArchive& archive);

	enum ConsistencyState
	{
		CS_UNKNOWN,
		CS_OK,
		CS_OPTIONAL,
		CS_INVALID
	};
	int CheckAttributeConsistency(const icomp::IRegistryElement& element, const std::string& attributeId);

	// reimplemented (icomp::IRegistryGeometryProvider)
	virtual i2d::CVector2d GetComponentPosition(const std::string& componentRole) const;
	virtual void SetComponentPosition(const std::string& componentRole, const i2d::CVector2d& point);

	// reimplemented (icomp::IRegistryNotesProvider)
	virtual istd::CString GetComponentNote(const std::string& componentRole) const;
	virtual void SetComponentNote(const std::string& componentRole, const istd::CString& componentNote);
	virtual void RemoveComponentNote(const std::string& componentRole);

	// reimplemented (icomp::IComponent)
	virtual void OnComponentCreated();

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


#endif // !CRegistryModelComp_included

