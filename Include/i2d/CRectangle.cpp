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
:	m_horizontalRange(ref.m_horizontalRange),
	m_verticalRange(ref.m_verticalRange)
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


void CRectangle::Reset()
{
	m_horizontalRange.Reset();
	m_verticalRange.Reset();
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
	return GetRight() - GetLeft();
}


double CRectangle::GetHeight() const 
{
	return GetBottom() - GetTop();
}


CVector2d CRectangle::GetSize() const
{
	return CVector2d(GetRight() - GetLeft(), GetBottom() - GetTop());
}


bool CRectangle::IsIntersectedBy(const CRectangle& rect) const
{
	return	m_horizontalRange.IsIntersectedBy(rect.m_horizontalRange) && m_verticalRange.IsIntersectedBy(rect.m_verticalRange);
}


bool CRectangle::IsIntersectedBy(const CLine2d& line) const
{
	i2d::CLine2d topLine(m_horizontalRange.GetMinValue(), m_verticalRange.GetMinValue(), m_horizontalRange.GetMaxValue(), m_verticalRange.GetMinValue());
	i2d::CLine2d bottomLine(m_horizontalRange.GetMinValue(), m_verticalRange.GetMaxValue(), m_horizontalRange.GetMaxValue(), m_verticalRange.GetMaxValue());
	i2d::CLine2d leftLine(m_horizontalRange.GetMinValue(), m_verticalRange.GetMinValue(), m_horizontalRange.GetMinValue(), m_verticalRange.GetMaxValue());
	i2d::CLine2d rightLine(m_horizontalRange.GetMaxValue(), m_verticalRange.GetMinValue(), m_horizontalRange.GetMaxValue(), m_verticalRange.GetMaxValue());

	return (line.IsIntersectedBy(topLine) || line.IsIntersectedBy(bottomLine) || line.IsIntersectedBy(leftLine) || line.IsIntersectedBy(rightLine)); 
}


CRectangle CRectangle::GetIntersection(const CRectangle& rect) const
{
	double outputLeft = istd::Max(rect.GetLeft(), GetLeft());
	double outputTop = istd::Max(rect.GetTop(), GetTop());
	double outputRight = istd::Min(rect.GetRight(), GetRight());
	double outputBottom = istd::Min(rect.GetBottom(), GetBottom());

	return CRectangle(outputLeft, outputTop, outputRight - outputLeft, outputBottom - outputTop);
}


CLine2d CRectangle::GetIntersection(const CLine2d& line) const
{
	return line;
}


void CRectangle::Intersect(const CRectangle& rectangle)
{
	*this = GetIntersection(rectangle);
}


CRectangle CRectangle::GetUnion(const CRectangle& rect) const
{
	double outputLeft = istd::Min(rect.GetLeft(), GetLeft());
	double outputTop = istd::Min(rect.GetTop(), GetTop());
	double outputRight = istd::Max(rect.GetRight(), GetRight());
	double outputBottom = istd::Max(rect.GetBottom(), GetBottom());

	return CRectangle(outputLeft, outputTop, outputRight - outputLeft, outputBottom - outputTop);
}


void CRectangle::Unite(const CRectangle& rect)
{
	istd::CChangeNotifier changePtr(this);

	double outputLeft = istd::Min(rect.GetLeft(), GetLeft());
	double outputTop = istd::Min(rect.GetTop(), GetTop());
	double outputRight = istd::Max(rect.GetRight(), GetRight());
	double outputBottom = istd::Max(rect.GetBottom(), GetBottom());

	m_horizontalRange = istd::CRange(outputLeft, outputRight);
	m_verticalRange = istd::CRange(outputTop, outputBottom);
}


CRectangle CRectangle::GetUnion(const i2d::CVector2d& position) const
{
	CRectangle retVal = *this;

	retVal.Unite(position);

	return retVal;
}


void CRectangle::Unite(const i2d::CVector2d& position)
{
	double left = GetLeft();
	double right = GetRight();
	double top = GetTop();
	double bottom = GetBottom();

	if (position.GetX() < left){
		SetLeft(position.GetX());
	}
	else if (position.GetX() > right){
		SetRight(position.GetX());
	}
	if (position.GetY() < top){
		SetTop(position.GetY());
	}
	else if (position.GetY() > bottom){
		SetBottom(position.GetY());
	}
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


bool CRectangle::operator==(const CRectangle& rect) const
{
	return ((m_horizontalRange == rect.m_horizontalRange) && (m_verticalRange == rect.m_verticalRange));
}
	

bool CRectangle::operator!=(const CRectangle& rect) const
{
	return !operator==(rect);
}


CRectangle CRectangle::GetBoundingBox() const
{
	return *this;
}

	
CVector2d CRectangle::GetNearestPointTo(const CVector2d& point) const
{
	return CVector2d(m_horizontalRange.GetNearestInside(point.GetX()), m_verticalRange.GetNearestInside(point.GetY()));
}


CRectangle CRectangle::GetTranslated(const i2d::CVector2d& delta) const
{
	CRectangle copy(*this);
	copy.Translate(delta);
	return copy;
}


// reimplemented (i2d::IObject2d)

CVector2d CRectangle::GetCenter() const
{
	return (GetTopLeft() + GetBottomRight()) * 0.5;
}


void CRectangle::MoveCenterTo(const CVector2d& position)
{
	i2d::CVector2d offset = position - GetCenter();
	if (offset != i2d::CVector2d(0, 0)){
		istd::CChangeNotifier notifier(this, i2d::IObject2d::CF_OBJECT_POSITION | istd::IChangeable::CF_MODEL);

		m_verticalRange.SetMinValue(m_verticalRange.GetMinValue() + offset.GetY());
		m_verticalRange.SetMaxValue(m_verticalRange.GetMaxValue() + offset.GetY());

		m_horizontalRange.SetMinValue(m_horizontalRange.GetMinValue() + offset.GetX());
		m_horizontalRange.SetMaxValue(m_horizontalRange.GetMaxValue() + offset.GetX());
	}
}


bool CRectangle::Transform(
			const ITransformation2d& /*transformation*/,
			ITransformation2d::ExactnessMode /*mode*/,
			double* /*errorFactorPtr*/)
{
	// TODO: implement geometrical transformations for rectangle.
	return false;
}


bool CRectangle::InvTransform(
			const ITransformation2d& /*transformation*/,
			ITransformation2d::ExactnessMode /*mode*/,
			double* /*errorFactorPtr*/)
{
	// TODO: implement geometrical transformations for rectangle.
	return false;
}


bool CRectangle::GetTransformed(
			const ITransformation2d& /*transformation*/,
			IObject2d& /*result*/,
			ITransformation2d::ExactnessMode /*mode*/,
			double* /*errorFactorPtr*/) const
{
	// TODO: implement geometrical transformations for rectangle.
	return false;
}


bool CRectangle::GetInvTransformed(
			const ITransformation2d& /*transformation*/,
			IObject2d& /*result*/,
			ITransformation2d::ExactnessMode /*mode*/,
			double* /*errorFactorPtr*/) const
{
	// TODO: implement geometrical transformations for rectangle.
	return false;
}


// reimplemented (iser::ISerializable)

bool CRectangle::Serialize(iser::IArchive& archive)
{
	double& top = m_verticalRange.GetMinValueRef();
	double& bottom = m_verticalRange.GetMaxValueRef();
	double& left = m_horizontalRange.GetMinValueRef();
	double& right = m_horizontalRange.GetMaxValueRef();

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


