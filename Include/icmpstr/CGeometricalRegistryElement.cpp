#include "icmpstr/CGeometricalRegistryElement.h"


#include "istd/TChangeNotifier.h"

#include "i2d/IObject2d.h"

#include "icomp/CRegistryElement.h"

#include "icmpstr/icmpstr.h"


namespace icmpstr
{


CGeometricalRegistryElement::CGeometricalRegistryElement()
:	m_center(0, 0),
	m_registryPtr(NULL)
{
}


void CGeometricalRegistryElement::SetName(const std::string& name)
{
	if (name != m_name){
		istd::CChangeNotifier notifier(this);

		m_name = name;
	}
}


// overloaded (icomp::CRegistryElement)

void CGeometricalRegistryElement::Initialize(
			const icomp::IRegistry* parentPtr,
			const icomp::IComponentStaticInfo* infoPtr,
			const icomp::CComponentAddress& address)
{
	BaseClass::Initialize(infoPtr);

	m_registryPtr = parentPtr;
	m_addess = address;
}


// reimplemented (IObject2d)

i2d::CVector2d CGeometricalRegistryElement::GetCenter() const
{
	return m_center;
}


void CGeometricalRegistryElement::MoveTo(const i2d::CVector2d& position)
{
	if (position != m_center){
		istd::CChangeNotifier notifier(this);

		m_center = position;
	}
}


} // namespace icmpstr


