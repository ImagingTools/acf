#ifndef CRegistryModelComp_included
#define CRegistryModelComp_included


#include <QPoint>


#include "icomp/CRegistry.h"
#include "icomp/CPackageStaticInfo.h"
#include "icomp/CComponentBase.h"

#include "IRegistryGeometryProvider.h"


class CRegistryModelComp:	public icomp::CComponentBase,
							public icomp::CRegistry,
							virtual public IRegistryGeometryProvider
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
		CF_POSITION = 0x1
	};

	// reimplemented (IRegistryGeometryProvider)
	virtual QPoint GetComponentPosition(const istd::CString& componentRole) const;
	virtual void SetComponentPosition(const istd::CString& componentRole, const QPoint& point);

	// reimplemented (icomp::IComponent)
	virtual void OnComponentCreated();

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	bool SerializeComponentPosition(iser::IArchive& archive, istd::CString& componentRole, int& x, int& y);

private:
	I_REF(icomp::CPackageStaticInfo, m_staticInfoCompPtr);

	typedef std::map<istd::CString, QPoint> ElementsPositionMap;

	ElementsPositionMap m_elementsPositionMap;
};


#endif // !CRegistryModelComp_included


