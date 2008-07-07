#include "i2d/CLine2d.h"


#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"

#include "i2d/CRectangle.h"


namespace i2d
{


CLine2d::CLine2d()
{
}


CLine2d::CLine2d(const CVector2d& point1, const CVector2d& point2)
	:m_point1(point1),
	m_point2(point2)
{
}


CLine2d::CLine2d(double x1, double y1, double x2, double y2)
	:m_point1(x1,y1),
	m_point2(x2,y2)
{
}


CLine2d::CLine2d(const CLine2d& line)
	:m_point1(line.m_point1), 
	m_point2(line.m_point2)
{
}


CLine2d CLine2d::operator = (const CLine2d& inl)
{
	m_point1 = inl.m_point1;
	m_point2 = inl.m_point2;

	return *this;
}


inline const CVector2d& CLine2d::GetPoint1() const
{
	return m_point1;
}


inline const CVector2d& CLine2d::GetPoint2() const
{
	return m_point2;
}


inline double CLine2d::GetSlope() const
{
	double y1 = m_point1.GetY();
	double y2 = m_point2.GetY();
	double x1 = m_point1.GetX();
	double x2 = m_point2.GetX();

	if(x2 == x1){
		return I_INFINITY;
	}

	return (y2 - y1) / (x2 - x1);
}


inline double CLine2d::GetIntercept() const
{
	double y1 = m_point1.GetY();
	double y2 = m_point2.GetY();
	double x1 = m_point1.GetX();
	double x2 = m_point2.GetX();

	if(x2 == x1){
		return I_INFINITY;
	}

	return -(-x2 * y1 + y2 * x1) / (x2 - x1);
}


bool CLine2d::IsParalell(const CLine2d& line) const
{
	return !IsIntersectedBy(line, false);
}


bool CLine2d::IsIntersectedBy(const CLine2d& line, CVector2d* intersectionPtr) const
{
	CVector2d a = m_point2 - m_point1;
	CVector2d b = line.m_point2 - line.m_point1;
	CVector2d c = line.m_point1 - m_point1;

	double scale = fabs(a.GetCrossProductZ(b));
	if (fabs(scale) < I_EPSILON){
		return false;
	}

	if (intersectionPtr != NULL){
		*intersectionPtr =  m_point1 + a * ((c.GetCrossProductZ(b) * a.GetCrossProductZ(b)) / (scale * scale));
	}

	return true;
}


CVector2d CLine2d::GetIntersection(const CLine2d& line) const
{
	CVector2d intersectionPoint;

	IsIntersectedBy(line, &intersectionPoint);

	return intersectionPoint;
}



// reimplemented (IObject2d)

CVector2d CLine2d::GetCenter() const
{
	return GetBoundingBox().GetCenter();
}


CRectangle CLine2d::GetBoundingBox() const
{
	double left = istd::Min(m_point1.GetX(), m_point2.GetX());
	double top = istd::Min(m_point1.GetY(), m_point2.GetY());
	double right = istd::Max(m_point1.GetX(), m_point2.GetX());
	double bottom = istd::Max(m_point1.GetY(), m_point2.GetY());

	return CRectangle(top, left, bottom, right);
}


// reimplemented (iser::ISerializable)

bool CLine2d::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag beginPointTag("BeginPoint", "BeginPoint");

	bool retVal = archive.BeginTag(beginPointTag);
	retVal = retVal && m_point1.Serialize(archive);
	retVal = retVal && archive.EndTag(beginPointTag);

	static iser::CArchiveTag endPointTag("EndPoint", "EndPoint");
	retVal = retVal && archive.BeginTag(endPointTag);
	retVal = retVal && m_point2.Serialize(archive);
	retVal = retVal && archive.EndTag(endPointTag);

	return retVal;
}


} // namespace i2d

