#include "i2d/CAnnulus.h"


#include "istd/TChangeNotifier.h"

#include "i2d/CRectangle.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace i2d
{


CAnnulus::CAnnulus()
{
}


CAnnulus::CAnnulus(const i2d::CVector2d& center, double innerRadius, double outerRadius)
:	BaseClass(center),
	m_radiusRange(innerRadius, outerRadius)
{
	I_ASSERT(IsAnnulusValid());
}


bool CAnnulus::IsAnnulusValid() const
{
	return m_radiusRange.IsValid();
}


double CAnnulus::GetInnerRadius() const
{
	return m_radiusRange.GetMinValue();
}


void CAnnulus::SetInnerRadius(double innerRadius)
{
	if (m_radiusRange.GetMinValue() != innerRadius && innerRadius <= m_radiusRange.GetMaxValue()){
		istd::CChangeNotifier changePtr(this);
	
		m_radiusRange.SetMinValue(innerRadius);
	}
}


double CAnnulus::GetOuterRadius() const
{
	return m_radiusRange.GetMaxValue();
}


void CAnnulus::SetOuterRadius(double outerRadius)
{
	if (m_radiusRange.GetMaxValue() != outerRadius && outerRadius >= m_radiusRange.GetMinValue()){
		istd::CChangeNotifier changePtr(this);
	
		m_radiusRange.SetMaxValue(outerRadius);
	}
}


CCircle CAnnulus::GetInnerCircle() const
{
	return CCircle(m_radiusRange.GetMinValue(), m_position);
}


CCircle CAnnulus::GetOuterCircle() const
{
	return CCircle(m_radiusRange.GetMaxValue(), m_position);
}


CRectangle CAnnulus::GetBoundingBox() const
{
	double outerRadius = m_radiusRange.GetMaxValue();

	return CRectangle(m_position.GetX() - outerRadius, 
		m_position.GetY() - outerRadius,
		2 * outerRadius,
		2 * outerRadius);
}


bool CAnnulus::operator == (const CAnnulus & ref) const
{
	double deltaInner = qAbs( ref.GetInnerRadius() - GetInnerRadius() );
	double deltaOuter = qAbs( ref.GetOuterRadius() - GetOuterRadius() );

	return (deltaInner < I_BIG_EPSILON && deltaOuter < I_BIG_EPSILON) ? true : false;
}


bool CAnnulus::operator != (const CAnnulus & ref) const
{
	return !operator ==(ref);
}


// reimplemented (iser::ISerializable)

bool CAnnulus::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag innerRadiusTag("InnerRadius", "Inner radius");
	static iser::CArchiveTag outerRadiusTag("OuterRadius", "Outer radius");

	istd::CChangeNotifier notifier(archive.IsStoring()? NULL: this, CF_OBJECT_POSITION | istd::IChangeable::CF_MODEL);

	bool retVal = BaseClass::Serialize(archive);

	double innerRadius = m_radiusRange.GetMinValue();
	double outerRadius = m_radiusRange.GetMaxValue();

	retVal = retVal && archive.BeginTag(innerRadiusTag);
	retVal = retVal && archive.Process(innerRadius);
	retVal = retVal && archive.EndTag(innerRadiusTag);

	retVal = retVal && archive.BeginTag(outerRadiusTag);
	retVal = retVal && archive.Process(outerRadius);
	retVal = retVal && archive.EndTag(outerRadiusTag);

	if (!archive.IsStoring()){
		istd::CChangeNotifier changePtr(this);

		m_radiusRange = istd::CRange(innerRadius, outerRadius);
	}

	return retVal;
}


} // namespace i2d


