#include "icmpstr/CVisualRegistryElement.h"


#include "istd/TChangeNotifier.h"

#include "i2d/IObject2d.h"

#include "icomp/CRegistryElement.h"

#include "icmpstr/icmpstr.h"


namespace icmpstr
{


CVisualRegistryElement::CVisualRegistryElement()
:	m_center(0, 0),
	m_registryPtr(NULL)
{
}


void CVisualRegistryElement::SetName(const std::string& name)
{
	m_name = name;
}


// overloaded (icomp::CRegistryElement)

void CVisualRegistryElement::Initialize(
			const icomp::IRegistry* parentPtr,
			const icomp::CComponentAddress& address)
{
	m_registryPtr = parentPtr;
	m_addess = address;
}


// reimplemented (IObject2d)

i2d::CVector2d CVisualRegistryElement::GetCenter() const
{
	return m_center;
}


void CVisualRegistryElement::MoveCenterTo(const i2d::CVector2d& position)
{
	if (position != m_center){
		istd::CChangeNotifier notifier(this, i2d::IObject2d::CF_OBJECT_POSITION | istd::IChangeable::CF_MODEL);

		m_center = position;
	}
}


bool CVisualRegistryElement::Transform(
			const i2d::ITransformation2d& /*transformation*/,
			i2d::ITransformation2d::ExactnessMode /*mode*/,
			double* /*errorFactorPtr*/)
{
	return false;
}


bool CVisualRegistryElement::InvTransform(
			const i2d::ITransformation2d& /*transformation*/,
			i2d::ITransformation2d::ExactnessMode /*mode*/,
			double* /*errorFactorPtr*/)
{
	return false;
}


bool CVisualRegistryElement::GetTransformed(
			const i2d::ITransformation2d& /*transformation*/,
			i2d::IObject2d& /*result*/,
			i2d::ITransformation2d::ExactnessMode /*mode*/,
			double* /*errorFactorPtr*/) const
{
	return false;
}


bool CVisualRegistryElement::GetInvTransformed(
			const i2d::ITransformation2d& /*transformation*/,
			i2d::IObject2d& /*result*/,
			i2d::ITransformation2d::ExactnessMode /*mode*/,
			double* /*errorFactorPtr*/) const
{
	return false;
}


} // namespace icmpstr


