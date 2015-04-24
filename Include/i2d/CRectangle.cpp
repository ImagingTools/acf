#include "i2d/CRectangle.h"


// ACF includes
#include "istd/CChangeNotifier.h"

#include "i2d/CVector2d.h"
#include "i2d/CRect.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"
#include "istd/TDelPtr.h"


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


CRectangle::CRectangle(const i2d::CRect& rect)
:	m_horizontalRange(rect.GetLeft(), rect.GetRight()),
	m_verticalRange(rect.GetTop(), rect.GetBottom())
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


bool CRectangle::IsValidNonEmpty() const
{
	return m_horizontalRange.IsValidNonEmpty() && m_verticalRange.IsValidNonEmpty();
}


void CRectangle::Reset()
{
	m_horizontalRange.Reset();
	m_verticalRange.Reset();
}


void CRectangle::SetLeft(double left)
{
	if (GetLeft() != left){
		BeginChanges(GetAnyChange());

		m_horizontalRange.SetMinValue(left);

		EndChanges(GetAnyChange());
	}
}


void CRectangle::SetTop(double top)
{
	if (GetTop() != top){
		BeginChanges(GetAnyChange());

		m_verticalRange.SetMinValue(top);

		EndChanges(GetAnyChange());
	}
}


void CRectangle::SetRight(double right)
{
	if (GetRight() != right){
		BeginChanges(GetAnyChange());

		m_horizontalRange.SetMaxValue(right);

		EndChanges(GetAnyChange());
	}
}


void CRectangle::SetBottom(double bottom)
{
	if (GetBottom() != bottom){
		BeginChanges(GetAnyChange());

		m_verticalRange.SetMaxValue(bottom);

		EndChanges(GetAnyChange());
	}
}


void CRectangle::SetHorizontalRange(const istd::CRange& range)
{
	if (range != m_horizontalRange){
		BeginChanges(GetAnyChange());

		m_horizontalRange = range;

		EndChanges(GetAnyChange());
	}
}


void CRectangle::SetVerticalRange(const istd::CRange& range)
{
	if (range != m_verticalRange){
		BeginChanges(GetAnyChange());

		m_verticalRange = range;

		EndChanges(GetAnyChange());
	}
}


CVector2d CRectangle::GetLeftTop() const
{
	return CVector2d(m_horizontalRange.GetMinValue(), m_verticalRange.GetMinValue()); 
}


void CRectangle::SetTopLeft(const CVector2d& topLeft)
{
	if (topLeft != GetLeftTop()){
		BeginChanges(GetAnyChange());

		m_horizontalRange.SetMinValue(topLeft.GetX());
		m_verticalRange.SetMinValue(topLeft.GetY());

		EndChanges(GetAnyChange());
	}
}


CVector2d CRectangle::GetRightTop() const
{
	return CVector2d(m_horizontalRange.GetMaxValue(), m_verticalRange.GetMinValue()); 
}


void CRectangle::SetTopRight(const CVector2d& topRight)
{
	if (topRight != GetRightTop()){
		BeginChanges(GetAnyChange());

		m_horizontalRange.SetMaxValue(topRight.GetX());
		m_verticalRange.SetMinValue(topRight.GetY());

		EndChanges(GetAnyChange());
	}
}


CVector2d CRectangle::GetLeftBottom() const
{
	return CVector2d(m_horizontalRange.GetMinValue(), m_verticalRange.GetMaxValue()); 
}


void CRectangle::SetBottomLeft(const CVector2d& bottomLeft)
{
	if (bottomLeft != GetLeftBottom()){
		BeginChanges(GetAnyChange());

		m_horizontalRange.SetMinValue(bottomLeft.GetX());
		m_verticalRange.SetMaxValue(bottomLeft.GetY());

		EndChanges(GetAnyChange());
	}
}


CVector2d CRectangle::GetRightBottom() const
{
	return CVector2d(m_horizontalRange.GetMaxValue(), m_verticalRange.GetMaxValue()); 
}


void CRectangle::SetBottomRight(const CVector2d& bottomRight)
{
	if (bottomRight != GetRightBottom()){
		BeginChanges(GetAnyChange());

		m_horizontalRange.SetMaxValue(bottomRight.GetX());
		m_verticalRange.SetMaxValue(bottomRight.GetY());

		EndChanges(GetAnyChange());
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
	return m_horizontalRange.IsIntersectedBy(rect.m_horizontalRange) && m_verticalRange.IsIntersectedBy(rect.m_verticalRange);
}


bool CRectangle::IsIntersectedBy(const CLine2d& line) const
{
	const i2d::CVector2d& point1 = line.GetPoint1();
	const i2d::CVector2d& point2 = line.GetPoint2();

	bool isPoint1OutsideLeft = point1.GetX() < m_horizontalRange.GetMinValue();
	bool isPoint1OutsideRight = point1.GetX() > m_horizontalRange.GetMaxValue();
	bool isPoint1OutsideTop = point1.GetY() < m_verticalRange.GetMinValue();
	bool isPoint1OutsideBottom = point1.GetY() > m_verticalRange.GetMaxValue();

	if (!isPoint1OutsideLeft && !isPoint1OutsideRight && !isPoint1OutsideTop && !isPoint1OutsideBottom){
		return true;
	}

	bool isPoint2OutsideLeft = point2.GetX() < m_horizontalRange.GetMinValue();
	bool isPoint2OutsideRight = point2.GetX() > m_horizontalRange.GetMaxValue();
	bool isPoint2OutsideTop = point2.GetY() < m_verticalRange.GetMinValue();
	bool isPoint2OutsideBottom = point2.GetY() > m_verticalRange.GetMaxValue();

	if (!isPoint2OutsideLeft && !isPoint2OutsideRight && !isPoint2OutsideTop && !isPoint2OutsideBottom){
		return true;
	}

	if (		(isPoint1OutsideLeft && isPoint2OutsideLeft) ||
				(isPoint1OutsideRight && isPoint2OutsideRight) ||
				(isPoint1OutsideTop && isPoint2OutsideTop) ||
				(isPoint1OutsideBottom && isPoint2OutsideBottom)){
		return false;
	}

	i2d::CLine2d topLine(m_horizontalRange.GetMinValue(), m_verticalRange.GetMinValue(), m_horizontalRange.GetMaxValue(), m_verticalRange.GetMinValue());
	i2d::CLine2d bottomLine(m_horizontalRange.GetMinValue(), m_verticalRange.GetMaxValue(), m_horizontalRange.GetMaxValue(), m_verticalRange.GetMaxValue());
	i2d::CLine2d leftLine(m_horizontalRange.GetMinValue(), m_verticalRange.GetMinValue(), m_horizontalRange.GetMinValue(), m_verticalRange.GetMaxValue());
	i2d::CLine2d rightLine(m_horizontalRange.GetMaxValue(), m_verticalRange.GetMinValue(), m_horizontalRange.GetMaxValue(), m_verticalRange.GetMaxValue());

	return (line.IsIntersectedBy(topLine) || line.IsIntersectedBy(bottomLine) || line.IsIntersectedBy(leftLine) || line.IsIntersectedBy(rightLine)); 
}


CRectangle CRectangle::GetIntersection(const CRectangle& rect) const
{
	double outputLeft = qMax(rect.GetLeft(), GetLeft());
	double outputTop = qMax(rect.GetTop(), GetTop());
	double outputRight = qMin(rect.GetRight(), GetRight());
	double outputBottom = qMin(rect.GetBottom(), GetBottom());

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
	double outputLeft = qMin(rect.GetLeft(), GetLeft());
	double outputTop = qMin(rect.GetTop(), GetTop());
	double outputRight = qMax(rect.GetRight(), GetRight());
	double outputBottom = qMax(rect.GetBottom(), GetBottom());

	return CRectangle(outputLeft, outputTop, outputRight - outputLeft, outputBottom - outputTop);
}


void CRectangle::Unite(const CRectangle& rect)
{
	BeginChanges(GetAnyChange());

	double outputLeft = qMin(rect.GetLeft(), GetLeft());
	double outputTop = qMin(rect.GetTop(), GetTop());
	double outputRight = qMax(rect.GetRight(), GetRight());
	double outputBottom = qMax(rect.GetBottom(), GetBottom());

	m_horizontalRange = istd::CRange(outputLeft, outputRight);
	m_verticalRange = istd::CRange(outputTop, outputBottom);

	EndChanges(GetAnyChange());
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
	BeginChanges(GetAnyChange());

	m_horizontalRange.Expand(rect.m_horizontalRange);
	m_verticalRange.Expand(rect.m_verticalRange);

	EndChanges(GetAnyChange());
}


double CRectangle::GetDistance(const CVector2d& position) const
{
	// Search block order:
	//
	//	 1   2   3
	//	   x---x
	//	 4 | 5 | 6
	//	   x---x
	//	 7   8   9

	if (position.GetY() < m_verticalRange.GetMinValue()){
		if (position.GetX() < m_horizontalRange.GetMinValue()){
			// Block 1
			return position.GetDistance(GetLeftTop());
		}
		else if (position.GetX() <= m_horizontalRange.GetMaxValue()){
			// Block 2
			return m_verticalRange.GetMinValue() - position.GetY();
		}
		else{
			// Block 3
			return position.GetDistance(GetRightTop());
		}
	}
	else if (position.GetY() <= m_verticalRange.GetMaxValue()){
		if (position.GetX() < m_horizontalRange.GetMinValue()){
			// Block 4
			return m_horizontalRange.GetMinValue() - position.GetX();
		}
		else if (position.GetX() <= m_horizontalRange.GetMaxValue()){
			// point is inside, Block 5
			return 0;
		}
		else{
			// Block 6
			return position.GetX() - m_horizontalRange.GetMaxValue();
		}
	}
	else{
		if (position.GetX() < m_horizontalRange.GetMinValue()){
			// Block 7
			return position.GetDistance(GetLeftBottom());
		}
		else if (position.GetX() <= m_horizontalRange.GetMaxValue()){
			// Block 8
			return position.GetY() - m_verticalRange.GetMaxValue();
		}
		else{
			// Block 9
			return position.GetDistance(GetRightBottom());
		}
	}
}


double CRectangle::GetMaxDistance(const CVector2d& position) const
{
	double maxDistanceTop2 = qMax(position.GetDistance2(GetLeftTop()), position.GetDistance2(GetRightTop()));
	double maxDistanceBottom2 = qMax(position.GetDistance2(GetLeftBottom()), position.GetDistance2(GetRightBottom()));

	return qSqrt(qMax(maxDistanceTop2, maxDistanceBottom2));
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


void CRectangle::Translate(const i2d::CVector2d& delta)
{
	BeginChanges(s_objectPositionAllDataChangeSet);

	m_horizontalRange.SetMinValue(m_horizontalRange.GetMinValue() + delta.GetX());
	m_verticalRange.SetMinValue(m_verticalRange.GetMinValue() + delta.GetY());
	m_horizontalRange.SetMaxValue(m_horizontalRange.GetMaxValue() + delta.GetX());
	m_verticalRange.SetMaxValue(m_verticalRange.GetMaxValue() + delta.GetY());

	EndChanges(s_objectPositionAllDataChangeSet);
}


bool CRectangle::operator==(const CRectangle& rect) const
{
	return ((m_horizontalRange == rect.m_horizontalRange) && (m_verticalRange == rect.m_verticalRange));
}


bool CRectangle::operator!=(const CRectangle& rect) const
{
	return !operator==(rect);
}


CRectangle::operator QRectF() const
{
	return QRectF(
				m_horizontalRange.GetMinValue(),
				m_verticalRange.GetMinValue(),
				m_horizontalRange.GetMinValue(),
				m_verticalRange.GetMinValue());
}


// reimplemented (i2d::IObject2d)

CVector2d CRectangle::GetCenter() const
{
	return (GetLeftTop() + GetRightBottom()) * 0.5;
}


void CRectangle::MoveCenterTo(const CVector2d& position)
{
	i2d::CVector2d offset = position - GetCenter();
	if (offset != i2d::CVector2d(0, 0)){
		BeginChanges(s_objectPositionChangeSet);

		m_verticalRange.SetMinValue(m_verticalRange.GetMinValue() + offset.GetY());
		m_verticalRange.SetMaxValue(m_verticalRange.GetMaxValue() + offset.GetY());

		m_horizontalRange.SetMinValue(m_horizontalRange.GetMinValue() + offset.GetX());
		m_horizontalRange.SetMaxValue(m_horizontalRange.GetMaxValue() + offset.GetX());

		EndChanges(s_objectPositionChangeSet);
	}
}


CRectangle CRectangle::GetBoundingBox() const
{
	return *this;
}

	
bool CRectangle::Transform(
			const ITransformation2d& transformation,
			ITransformation2d::ExactnessMode mode,
			double* errorFactorPtr)
{
	i2d::CVector2d leftTop (m_horizontalRange.GetMinValue(), m_verticalRange.GetMinValue());
	i2d::CVector2d rightBottom (m_horizontalRange.GetMaxValue(), m_verticalRange.GetMaxValue());

	i2d::CVector2d transLeftTop;
	i2d::CVector2d transRightBottom;

	if (		transformation.GetPositionAt(leftTop, transLeftTop, mode) &&
				transformation.GetPositionAt(rightBottom, transRightBottom, mode)){
		BeginChanges(s_objectPositionAllDataChangeSet);
		
		if (errorFactorPtr != NULL){
			*errorFactorPtr = 0;
		}

		m_horizontalRange = istd::CRange(transLeftTop.GetX(), transRightBottom.GetX());
		m_horizontalRange.Validate();

		m_verticalRange = istd::CRange(transLeftTop.GetY(), transRightBottom.GetY());
		m_verticalRange.Validate();

		EndChanges(s_objectPositionAllDataChangeSet);

		return true;
	}

	return false;
}


bool CRectangle::InvTransform(
			const ITransformation2d& transformation,
			ITransformation2d::ExactnessMode mode,
			double* errorFactorPtr)
{
	i2d::CVector2d leftTop (m_horizontalRange.GetMinValue(), m_verticalRange.GetMinValue());
	i2d::CVector2d rightBottom (m_horizontalRange.GetMaxValue(), m_verticalRange.GetMaxValue());

	i2d::CVector2d transLeftTop;
	i2d::CVector2d transRightBottom;

	if (		transformation.GetInvPositionAt(leftTop, transLeftTop, mode) &&
				transformation.GetInvPositionAt(rightBottom, transRightBottom, mode)){
		BeginChanges(s_objectPositionAllDataChangeSet);
		
		if (errorFactorPtr != NULL){
			*errorFactorPtr = 0;
		}

		m_horizontalRange = istd::CRange(transLeftTop.GetX(), transRightBottom.GetX());
		m_verticalRange = istd::CRange(transLeftTop.GetY(), transRightBottom.GetY());

		m_horizontalRange.Validate();
		m_verticalRange.Validate();

		EndChanges(s_objectPositionAllDataChangeSet);

		return true;
	}

	return false;
}


bool CRectangle::GetTransformed(
			const ITransformation2d& transformation,
			IObject2d& result,
			ITransformation2d::ExactnessMode mode,
			double* errorFactorPtr) const
{
	CRectangle* resultRectanglePtr = dynamic_cast<CRectangle*>(&result);
	if (resultRectanglePtr != NULL){
		return resultRectanglePtr->Transform(transformation, mode, errorFactorPtr);
	}

	return false;
}


bool CRectangle::GetInvTransformed(
			const ITransformation2d& transformation,
			IObject2d& result,
			ITransformation2d::ExactnessMode mode,
			double* errorFactorPtr) const
{
	CRectangle* resultRectanglePtr = dynamic_cast<CRectangle*>(&result);
	if (resultRectanglePtr != NULL){
		return resultRectanglePtr->InvTransform(transformation, mode, errorFactorPtr);
	}

	return false;
}


// reimplemented (istd::IChangeable)

int CRectangle::GetSupportedOperations() const
{
	return SO_COPY | SO_CLONE;
}


bool CRectangle::CopyFrom(const IChangeable& object, CompatibilityMode mode)
{
	const CRectangle* rectanglePtr = dynamic_cast<const CRectangle*>(&object);

	if (rectanglePtr != NULL){
		BeginChanges(GetAnyChange());

		SetHorizontalRange(rectanglePtr->GetHorizontalRange());
		SetVerticalRange(rectanglePtr->GetVerticalRange());

		CObject2dBase::CopyFrom(object, mode);

		EndChanges(GetAnyChange());

		return true;
	}

	return false;
}


istd::IChangeable* CRectangle::CloneMe(CompatibilityMode mode) const 
{
	istd::TDelPtr<CRectangle> clonePtr(new CRectangle);

	if (clonePtr->CopyFrom(*this, mode)){
		return clonePtr.PopPtr();
	}

	return NULL;
}


// reimplemented (iser::ISerializable)

bool CRectangle::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag topTag("Top", "Rectangle top edge position", iser::CArchiveTag::TT_LEAF);
	static iser::CArchiveTag bottomTag("Bottom", "Rectangle bottom edge position", iser::CArchiveTag::TT_LEAF);
	static iser::CArchiveTag leftTag("Left", "Rectangle left edge position", iser::CArchiveTag::TT_LEAF);
	static iser::CArchiveTag rightTag("Right", "Rectangle right edge position", iser::CArchiveTag::TT_LEAF);

	istd::CChangeNotifier notifier(archive.IsStoring()? NULL: this, &GetAllChanges());
	Q_UNUSED(notifier);

	double& top = m_verticalRange.GetMinValueRef();
	double& bottom = m_verticalRange.GetMaxValueRef();
	double& left = m_horizontalRange.GetMinValueRef();
	double& right = m_horizontalRange.GetMaxValueRef();

	bool retVal = archive.BeginTag(topTag);
	retVal = retVal && archive.Process(top);
	retVal = retVal && archive.EndTag(topTag);

	retVal = retVal && archive.BeginTag(bottomTag);
	retVal = retVal && archive.Process(bottom);
	retVal = retVal && archive.EndTag(bottomTag);

	retVal = retVal && archive.BeginTag(leftTag);
	retVal = retVal && archive.Process(left);
	retVal = retVal && archive.EndTag(leftTag);

	retVal = retVal && archive.BeginTag(rightTag);
	retVal = retVal && archive.Process(right);
	retVal = retVal && archive.EndTag(rightTag);
	
	return retVal;
}


// static attributes

CRectangle CRectangle::s_empty(0, 0, 0, 0);


} // namespace i2d


