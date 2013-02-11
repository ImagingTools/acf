#include "icmpstr/CVisualRegistryElement.h"


// ACF includes
#include "istd/TChangeNotifier.h"
#include "i2d/CRectangle.h"
#include "icomp/CRegistryElement.h"

#include "icmpstr/icmpstr.h"


namespace icmpstr
{


CVisualRegistryElement::CVisualRegistryElement()
:	m_center(0, 0),
	m_registryPtr(NULL)
{
}


void CVisualRegistryElement::SetName(const QByteArray& name)
{
	m_name = name;
}


const QString& CVisualRegistryElement::GetNote() const
{
	return m_note;
}


void CVisualRegistryElement::SetNote(const QString& note)
{
	m_note = note;
}


// overloaded (icomp::CRegistryElement)

void CVisualRegistryElement::Initialize(
			const icomp::IRegistry* parentPtr,
			const icomp::CComponentAddress& address)
{
	m_registryPtr = parentPtr;
	m_address = address;
}


// reimplemented (i2d::IObject2d)

i2d::CVector2d CVisualRegistryElement::GetCenter() const
{
	return m_center;
}


void CVisualRegistryElement::MoveCenterTo(const i2d::CVector2d& position)
{
	if (position != m_center){
		istd::CChangeNotifier notifier(this, CF_OBJECT_POSITION | CF_MODEL);

		m_center = position;
	}
}


i2d::CRectangle CVisualRegistryElement::GetBoundingBox() const
{
	// dummy implementation using center only
	return i2d::CRectangle(m_center.GetX() - I_BIG_EPSILON, m_center.GetY() - I_BIG_EPSILON, I_BIG_EPSILON * 2, I_BIG_EPSILON * 2);
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


