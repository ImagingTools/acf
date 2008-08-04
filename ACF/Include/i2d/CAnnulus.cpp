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


CAnnulus::CAnnulus(double innerRadius, double outerRadius, const i2d::CVector2d& center)
	:BaseClass(center),
	m_radiusRange(innerRadius, outerRadius)
{
	I_ASSERT(IsValid());
}


bool CAnnulus::IsValid() const
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
	return CCircle(m_radiusRange.GetMinValue(), m_center);
}


CCircle CAnnulus::GetOuterCircle() const
{
	return CCircle(m_radiusRange.GetMaxValue(), m_center);
}


// reimplemented (i2d::IObject2d)

CRectangle CAnnulus::GetBoundingBox() const
{
	double outerRadius = m_radiusRange.GetMaxValue();

	return CRectangle(m_center.GetY() - outerRadius, 
		m_center.GetX() - outerRadius,
		m_center.GetY() + outerRadius,
		m_center.GetX() + outerRadius);
}


// reimplemented (iser::ISerializable)

bool CAnnulus::Serialize(iser::IArchive& archive)
{
	bool retVal = BaseClass::Serialize(archive);

	double innerRadius = m_radiusRange.GetMinValue();
	double outerRadius = m_radiusRange.GetMaxValue();

	static iser::CArchiveTag innerRadiusTag("InnerRadius", "Inner radius");
	retVal = retVal && archive.BeginTag(innerRadiusTag);
	retVal = retVal && archive.Process(innerRadius);
	retVal = retVal && archive.EndTag(innerRadiusTag);

	static iser::CArchiveTag outerRadiusTag("OuterRadius", "Outer radius");
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


