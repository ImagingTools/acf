#include "i2d/CRectangle.h"


#include "istd/TChangeNotifier.h"

#include "i2d/CVector2d.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace i2d
{	


CRectangle::CRectangle()
{
}


CRectangle::CRectangle(double left, double top, double width, double height)
:	m_horizontalRange(left, left + width),
	m_verticalRange(top, top + height)
{
}


CRectangle::CRectangle(const CRectangle& ref)
:	m_verticalRange(ref.m_verticalRange),
	m_horizontalRange(ref.m_horizontalRange)
{
}


CRectangle::CRectangle(const CVector2d& topLeft, const CVector2d& bottomRight)
:	m_horizontalRange(topLeft.GetX(), bottomRight.GetX()),
	m_verticalRange(topLeft.GetY(), bottomRight.GetY())
{
}


CRectangle::CRectangle(const istd::CIndex2d& size)
:	m_horizontalRange(0.0, size.GetX()),
	m_verticalRange(0.0, size.GetY())
{
}


const CRectangle& CRectangle::operator=(const CRectangle& ref)
{
	m_horizontalRange = ref.m_horizontalRange;
	m_verticalRange = ref.m_verticalRange;

	return *this;
}


bool CRectangle::Contains(const CVector2d& point) const
{
	return m_horizontalRange.Contains(point.GetX()) && m_verticalRange.Contains(point.GetY());
}


bool CRectangle::Contains(const istd::CIndex2d& point) const
{
	return m_horizontalRange.Contains(point.GetX()) && m_verticalRange.Contains(point.GetY());
}


bool CRectangle::Contains(const CRectangle& rect) const
{
	return m_horizontalRange.Contains(rect.m_horizontalRange) && m_verticalRange.Contains(rect.m_verticalRange);
}


double CRectangle::GetWidth() const
{
	return double(GetRight() - GetLeft());
}


double CRectangle::GetHeight() const 
{
	return double(GetBottom() - GetTop());
}


bool CRectangle::IsEmpty() const
{
	return (GetWidth() > 0 && GetHeight() > 0);
}


bool CRectangle::IsValid() const
{
	return (GetRight() >= GetLeft()) && (GetBottom() >= GetTop());
}


bool CRectangle::IsIntersectedBy(const CRectangle& other) const
{
	return	Contains(other.GetTopLeft()) || 
			Contains(other.GetBottomLeft()) || 
			Contains(other.GetTopRight()) || 
			Contains(other.GetBottomRight()) ;
}



CRectangle CRectangle::GetIntersection(const CRectangle& other) const
{
	double outputLeft = istd::Max(other.GetLeft(), GetLeft());
	double outputTop = istd::Max(other.GetTop(), GetTop());
	double outputRight = istd::Min(other.GetRight(), GetRight());
	double outputBottom = istd::Min(other.GetBottom(), GetBottom());

	return CRectangle(outputLeft, outputTop, outputRight - outputLeft, outputBottom - outputTop);
}


CRectangle CRectangle::GetUnion(const CRectangle& other) const
{
	double outputLeft = istd::Min(other.GetLeft(), GetLeft());
	double outputTop = istd::Min(other.GetTop(), GetTop());
	double outputRight = istd::Max(other.GetRight(), GetRight());
	double outputBottom = istd::Max(other.GetBottom(), GetBottom());

	return CRectangle(outputLeft, outputTop, outputRight - outputLeft, outputBottom - outputTop);
}

	
void CRectangle::Expand(double left, double right, double top, double bottom)
{
	istd::CChangeNotifier changePtr(this);

	m_horizontalRange = istd::CRange(GetLeft() - left, GetRight() + right);
	m_verticalRange = istd::CRange(GetTop() - top, GetBottom() + bottom);
}


CRectangle CRectangle::GetExpanded(double left, double right, double top, double bottom) const
{
	i2d::CRectangle retVal(*this);

	retVal.Expand(left, right, top, bottom);

	return retVal;
}


double CRectangle::GetLeft() const
{
	return m_horizontalRange.GetMinValue();
}


double CRectangle::GetTop() const
{
	return m_verticalRange.GetMinValue();
}


double CRectangle::GetRight() const
{
	return m_horizontalRange.GetMaxValue();
}


double CRectangle::GetBottom() const
{
	return m_verticalRange.GetMaxValue();
}


void CRectangle::SetLeft(double left)
{
	if (GetLeft() != left){
		istd::CChangeNotifier changePtr(this);

		m_horizontalRange.SetMinValue(left);
	}
}


void CRectangle::SetTop(double top)
{
	if (GetTop() != top){
		istd::CChangeNotifier changePtr(this);

		m_verticalRange.SetMinValue(top);
	}
}


void CRectangle::SetRight(double right)
{
	if (GetRight() != right){
		istd::CChangeNotifier changePtr(this);

		m_horizontalRange.SetMaxValue(right);
	}
}


void CRectangle::SetBottom(double bottom)
{
	if (GetBottom() != bottom){
			istd::CChangeNotifier changePtr(this);

		m_verticalRange.SetMaxValue(bottom);
	}
}


CVector2d CRectangle::GetTopLeft() const
{
	return CVector2d(m_horizontalRange.GetMinValue(), m_verticalRange.GetMinValue()); 
}


CVector2d CRectangle::GetBottomLeft() const
{
	return CVector2d(m_horizontalRange.GetMinValue(), m_verticalRange.GetMaxValue()); 
}


CVector2d CRectangle::GetTopRight() const
{
	return CVector2d(m_horizontalRange.GetMaxValue(), m_verticalRange.GetMinValue()); 
}


CVector2d CRectangle::GetBottomRight() const
{
	return CVector2d(m_horizontalRange.GetMaxValue(), m_verticalRange.GetMaxValue()); 
}


void CRectangle::SetTopLeft(const CVector2d& topLeft)
{
	if (GetTopLeft() != topLeft){
		istd::CChangeNotifier changePtr(this);

		m_horizontalRange.SetMinValue(topLeft.GetX());
		m_verticalRange.SetMinValue(topLeft.GetY());
	}
}


void CRectangle::SetBottomLeft(const CVector2d& bottomLeft)
{
	if (GetBottomLeft() != bottomLeft){
		istd::CChangeNotifier changePtr(this);

		m_horizontalRange.SetMinValue(bottomLeft.GetX());
		m_verticalRange.SetMaxValue(bottomLeft.GetY());
	}
}	


void CRectangle::SetTopRight(const CVector2d& topRight)
{
	if (GetTopRight() != topRight){
		istd::CChangeNotifier changePtr(this);

		m_horizontalRange.SetMaxValue(topRight.GetX());
		m_verticalRange.SetMinValue(topRight.GetY());
	}
}


void CRectangle::SetBottomRight(const CVector2d& bottomRight)
{
	if (GetBottomRight() != bottomRight){
		istd::CChangeNotifier changePtr(this);

		m_horizontalRange.SetMaxValue(bottomRight.GetX());
		m_verticalRange.SetMaxValue(bottomRight.GetY());
	}
}


// reimplemented (IObject2d)

CVector2d CRectangle::GetCenter() const
{
	return CVector2d(GetWidth() * 0.5, GetHeight() * 0.5);
}


CRectangle CRectangle::GetBoundingBox() const
{
	return *this;
}


void CRectangle::MoveTo(const CVector2d& position)
{
	i2d::CVector2d offset = position - GetCenter();

	m_verticalRange.SetMinValue(m_verticalRange.GetMinValue() + offset.GetY());
	m_verticalRange.SetMaxValue(m_verticalRange.GetMaxValue() + offset.GetY());

	m_horizontalRange.SetMinValue(m_horizontalRange.GetMinValue() + offset.GetX());
	m_horizontalRange.SetMaxValue(m_horizontalRange.GetMaxValue() + offset.GetX());
}


// reimplemented (iser::ISerializable)

bool CRectangle::Serialize(iser::IArchive& archive)
{
	double top = m_verticalRange.GetMinValue();
	double bottom = m_verticalRange.GetMaxValue();
	double left = m_horizontalRange.GetMinValue();
	double right = m_horizontalRange.GetMinValue();

	static iser::CArchiveTag topTag("Top", "Rectangle top edge position");

	bool retVal = archive.BeginTag(topTag);
	retVal = retVal && archive.Process(top);
	retVal = retVal && archive.EndTag(topTag);

	static iser::CArchiveTag bottomTag("Bottom", "Rectangle bottom edge position");
	retVal = retVal && archive.BeginTag(bottomTag);
	retVal = retVal && archive.Process(bottom);
	retVal = retVal && archive.EndTag(bottomTag);

	static iser::CArchiveTag leftTag("Left", "Rectangle left edge position");
	retVal = retVal && archive.BeginTag(leftTag);
	retVal = retVal && archive.Process(left);
	retVal = retVal && archive.EndTag(leftTag);

	static iser::CArchiveTag rightTag("Right", "Rectangle right edge position");
	retVal = retVal && archive.BeginTag(rightTag);
	retVal = retVal && archive.Process(right);
	retVal = retVal && archive.EndTag(rightTag);

	if (!archive.IsStoring()){
		m_verticalRange.SetMinValue(top);
		m_verticalRange.SetMaxValue(bottom);
		m_horizontalRange.SetMinValue(left);
		m_horizontalRange.SetMaxValue(right);
	}
	
	return retVal;
}

	
} // namespace i2d
