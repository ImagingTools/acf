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


CRectangle::CRectangle(const CRectangle& ref)
:	m_verticalRange(ref.m_verticalRange),
	m_horizontalRange(ref.m_horizontalRange)
{
}


CRectangle::CRectangle(double left, double top, double width, double height)
:	m_horizontalRange(left, left + width),
	m_verticalRange(top, top + height)
{
}


CRectangle::CRectangle(const CVector2d& topLeft, const CVector2d& bottomRight)
:	m_horizontalRange(topLeft.GetX(), bottomRight.GetX()),
	m_verticalRange(topLeft.GetY(), bottomRight.GetY())
{
}


CRectangle::CRectangle(const istd::CRange& horizontalRange, const istd::CRange& verticalRange)
:	m_horizontalRange(horizontalRange),
	m_verticalRange(verticalRange)
{
}


CRectangle::CRectangle(const istd::CIndex2d& size)
:	m_horizontalRange(0.0, size.GetX()),
	m_verticalRange(0.0, size.GetY())
{
}


bool CRectangle::IsValid() const
{
	return m_horizontalRange.IsValid() && m_verticalRange.IsValid();
}


bool CRectangle::IsEmpty() const
{
	return m_horizontalRange.IsEmpty() || m_verticalRange.IsEmpty();
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


void CRectangle::SetHorizontalRange(const istd::CRange& range)
{
	if (range != m_horizontalRange){
		istd::CChangeNotifier notifier(this);

		m_horizontalRange = range;
	}
}


void CRectangle::SetVerticalRange(const istd::CRange& range)
{
	if (range != m_verticalRange){
		istd::CChangeNotifier notifier(this);

		m_verticalRange = range;
	}
}


CVector2d CRectangle::GetTopLeft() const
{
	return CVector2d(m_horizontalRange.GetMinValue(), m_verticalRange.GetMinValue()); 
}


void CRectangle::SetTopLeft(const CVector2d& topLeft)
{
	if (topLeft != GetTopLeft()){
		istd::CChangeNotifier changePtr(this);

		m_horizontalRange.SetMinValue(topLeft.GetX());
		m_verticalRange.SetMinValue(topLeft.GetY());
	}
}


CVector2d CRectangle::GetTopRight() const
{
	return CVector2d(m_horizontalRange.GetMaxValue(), m_verticalRange.GetMinValue()); 
}


void CRectangle::SetTopRight(const CVector2d& topRight)
{
	if (topRight != GetTopRight()){
		istd::CChangeNotifier changePtr(this);

		m_horizontalRange.SetMaxValue(topRight.GetX());
		m_verticalRange.SetMinValue(topRight.GetY());
	}
}


CVector2d CRectangle::GetBottomLeft() const
{
	return CVector2d(m_horizontalRange.GetMinValue(), m_verticalRange.GetMaxValue()); 
}


void CRectangle::SetBottomLeft(const CVector2d& bottomLeft)
{
	if (bottomLeft != GetBottomLeft()){
		istd::CChangeNotifier changePtr(this);

		m_horizontalRange.SetMinValue(bottomLeft.GetX());
		m_verticalRange.SetMaxValue(bottomLeft.GetY());
	}
}	


CVector2d CRectangle::GetBottomRight() const
{
	return CVector2d(m_horizontalRange.GetMaxValue(), m_verticalRange.GetMaxValue()); 
}


void CRectangle::SetBottomRight(const CVector2d& bottomRight)
{
	if (bottomRight != GetBottomRight()){
		istd::CChangeNotifier changePtr(this);

		m_horizontalRange.SetMaxValue(bottomRight.GetX());
		m_verticalRange.SetMaxValue(bottomRight.GetY());
	}
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


bool CRectangle::IsIntersectedBy(const CRectangle& rect) const
{
	return	m_horizontalRange.IsIntersectedBy(rect.m_horizontalRange) && m_verticalRange.IsIntersectedBy(rect.m_verticalRange);
}



CRectangle CRectangle::GetIntersection(const CRectangle& rect) const
{
	double outputLeft = istd::Max(rect.GetLeft(), GetLeft());
	double outputTop = istd::Max(rect.GetTop(), GetTop());
	double outputRight = istd::Min(rect.GetRight(), GetRight());
	double outputBottom = istd::Min(rect.GetBottom(), GetBottom());

	return CRectangle(outputLeft, outputTop, outputRight - outputLeft, outputBottom - outputTop);
}


CRectangle CRectangle::GetUnion(const CRectangle& rect) const
{
	double outputLeft = istd::Min(rect.GetLeft(), GetLeft());
	double outputTop = istd::Min(rect.GetTop(), GetTop());
	double outputRight = istd::Max(rect.GetRight(), GetRight());
	double outputBottom = istd::Max(rect.GetBottom(), GetBottom());

	return CRectangle(outputLeft, outputTop, outputRight - outputLeft, outputBottom - outputTop);
}


CRectangle CRectangle::GetExpanded(const CRectangle& rect) const
{
	return i2d::CRectangle(
				m_horizontalRange.GetExpanded(rect.m_horizontalRange),
				m_verticalRange.GetExpanded(rect.m_verticalRange));
}


void CRectangle::Expand(const CRectangle& rect)
{
	istd::CChangeNotifier changePtr(this);

	m_horizontalRange.Expand(rect.m_horizontalRange);
	m_verticalRange.Expand(rect.m_verticalRange);
}


const CRectangle& CRectangle::operator=(const CRectangle& rect)
{
	m_horizontalRange = rect.m_horizontalRange;
	m_verticalRange = rect.m_verticalRange;

	return *this;
}


// reimplemented (IObject2d)

CVector2d CRectangle::GetCenter() const
{
	return (GetTopLeft() + GetBottomRight()) * 0.5;
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
	double& top = m_verticalRange.GetMinValueRef();
	double& bottom = m_verticalRange.GetMaxValueRef();
	double& left = m_horizontalRange.GetMinValueRef();
	double& right = m_horizontalRange.GetMinValueRef();

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
	
	return retVal;
}

	
} // namespace i2d
