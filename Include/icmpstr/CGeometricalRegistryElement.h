#ifndef icmpstr_CGeometricalRegistryElement_included
#define icmpstr_CGeometricalRegistryElement_included


#include "i2d/IObject2d.h"

#include "icomp/IRegistry.h"
#include "icomp/CComponentAddress.h"
#include "icomp/CRegistryElement.h"

#include "icmpstr/icmpstr.h"


namespace icmpstr
{


class CGeometricalRegistryElement: public icomp::CRegistryElement, virtual public i2d::IObject2d
{
public:
	typedef icomp::CRegistryElement BaseClass;

	CGeometricalRegistryElement();

	const icomp::IRegistry* GetRegistry() const;
	const icomp::CComponentAddress& GetAddress() const;
	const std::string& GetName() const;
	void SetName(const std::string& name);

	// overloaded (icomp::CRegistryElement)
	void Initialize(
				const icomp::IRegistry* parentPtr,
				const icomp::IComponentStaticInfo* infoPtr,
				const icomp::CComponentAddress& address);

	// reimplemented (IObject2d)
	virtual i2d::CVector2d GetCenter() const;
	virtual void MoveTo(const i2d::CVector2d& position);

private:
	const icomp::IRegistry* m_registryPtr;
	icomp::CComponentAddress m_addess;
	i2d::CVector2d m_center;
	std::string m_name;
};


// inline methods

inline const icomp::IRegistry* CGeometricalRegistryElement::GetRegistry() const
{
	return m_registryPtr;
}


inline const icomp::CComponentAddress& CGeometricalRegistryElement::GetAddress() const
{
	return m_addess;
}


inline const std::string& CGeometricalRegistryElement::GetName() const
{
	return m_name;
}


} // namespace icmpstr


#endif //!icmpstr_CGeometricalRegistryElement_included


