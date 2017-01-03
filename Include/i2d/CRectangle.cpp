#include <i2d/CRectangle.h>


// Qt includes
#include <QtCore/QObject>

// ACF includes
#include <istd/TDelPtr.h>
#include <istd/CChangeNotifier.h>
#include <iser/IArchive.h>
#include <iser/CArchiveTag.h>
#include <i2d/CVector2d.h>
#include <i2d/CRect.h>


namespace i2d
{


// static constants
static const istd::IChangeable::ChangeSet s_resetRectangeChange(CRectangle::CF_OBJECT_POSITION, CRectangle::CF_ALL_DATA, QObject::tr("Reset rectangle"));
static const istd::IChangeable::ChangeSet s_setLeftSideChange(CRectangle::CF_OBJECT_POSITION, CRectangle::CF_ALL_DATA, QObject::tr("Set rectangle's left side"));
static const istd::IChangeable::ChangeSet s_setTopSideChange(CRectangle::CF_OBJECT_POSITION, CRectangle::CF_ALL_DATA, QObject::tr("Set rectangle's top side"));
static const istd::IChangeable::ChangeSet s_setRightSideChange(CRectangle::CF_OBJECT_POSITION, CRectangle::CF_ALL_DATA, QObject::tr("Set rectangle's right side"));
static const istd::IChangeable::ChangeSet s_setBottomSideChange(CRectangle::CF_OBJECT_POSITION, CRectangle::CF_ALL_DATA, QObject::tr("Set rectangle's bottom side"));
static const istd::IChangeable::ChangeSet s_setTopLeftChange(CRectangle::CF_OBJECT_POSITION, CRectangle::CF_ALL_DATA, QObject::tr("Set rectange's top-left position"));
static const istd::IChangeable::ChangeSet s_setTopRightChange(CRectangle::CF_OBJECT_POSITION, CRectangle::CF_ALL_DATA, QObject::tr("Set rectange's top-right position"));
static const istd::IChangeable::ChangeSet s_setBottomLeftChange(CRectangle::CF_OBJECT_POSITION, CRectangle::CF_ALL_DATA, QObject::tr("Set rectange's bottom-left position"));
static const istd::IChangeable::ChangeSet s_setBottomRightChange(CRectangle::CF_OBJECT_POSITION, CRectangle::CF_ALL_DATA, QObject::tr("Set rectange's bottom-right position"));
static const istd::IChangeable::ChangeSet s_uniteChange(CRectangle::CF_OBJECT_POSITION, CRectangle::CF_ALL_DATA, QObject::tr("Unite rectangle which another"));
static const istd::IChangeable::ChangeSet s_expandChange(CRectangle::CF_OBJECT_POSITION, CRectangle::CF_ALL_DATA, QObject::tr("Expand rectangle"));
static const istd::IChangeable::ChangeSet s_intersectChange(CRectangle::CF_OBJECT_POSITION, CRectangle::CF_ALL_DATA, QObject::tr("Intersect with another rectangle"));
static const iser::CArchiveTag s_topTag("Top", "Rectangle top edge position", iser::CArchiveTag::TT_LEAF);
static const iser::CArchiveTag s_bottomTag("Bottom", "Rectangle bottom edge position", iser::CArchiveTag::TT_LEAF);
static const iser::CArchiveTag s_leftTag("Left", "Rectangle left edge position", iser::CArchiveTag::TT_LEAF);
static const iser::CArchiveTag s_rightTag("Right", "Rectangle right edge position", iser::CArchiveTag::TT_LEAF);



// public methods

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
	istd::CChangeNotifier changeNotifier(this, &s_resetRectangeChange);
	Q_UNUSED(changeNotifier);

	m_horizontalRange.Reset();
	m_verticalRange.Reset();
}


void CRectangle::SetLeft(double left)
{
	if (GetLeft() != left){
		istd::CChangeNotifier changeNotifier(this, &s_setLeftSideChange);
		Q_UNUSED(changeNotifier);

		m_horizontalRange.SetMinValue(left);
	}
}


void CRectangle::SetTop(double top)
{
	if (GetTop() != top){
		istd::CChangeNotifier changeNotifier(this, &s_setTopSideChange);
		Q_UNUSED(changeNotifier);

		m_verticalRange.SetMinValue(top);
	}
}


void CRectangle::SetRight(double right)
{
	if (GetRight() != right){
		istd::CChangeNotifier changeNotifier(this, &s_setRightSideChange);
		Q_UNUSED(changeNotifier);

		m_horizontalRange.SetMaxValue(right);
	}
}


void CRectangle::SetBottom(double bottom)
{
	if (GetBottom() != bottom){
		istd::CChangeNotifier changeNotifier(this, &s_setBottomSideChange);
		Q_UNUSED(changeNotifier);

		m_verticalRange.SetMaxValue(bottom);
	}
}


void CRectangle::SetHorizontalRange(const istd::CRange& range)
{
	if (range != m_horizontalRange){
		istd::CChangeNotifier changeNotifier(this, &s_objectModifiedChange);
		Q_UNUSED(changeNotifier);

		m_horizontalRange = range;
	}
}


void CRectangle::SetVerticalRange(const istd::CRange& range)
{
	if (range != m_verticalRange){
		istd::CChangeNotifier changeNotifier(this, &s_objectModifiedChange);
		Q_UNUSED(changeNotifier);

		m_verticalRange = range;
	}
}


CVector2d CRectangle::GetLeftTop() const
{
	return CVector2d(m_horizontalRange.GetMinValue(), m_verticalRange.GetMinValue()); 
}


void CRectangle::SetTopLeft(const CVector2d& topLeft)
{
	if (topLeft != GetLeftTop()){
		istd::CChangeNotifier changeNotifier(this, &s_setTopLeftChange);
		Q_UNUSED(changeNotifier);

		m_horizontalRange.SetMinValue(topLeft.GetX());
		m_verticalRange.SetMinValue(topLeft.GetY());
	}
}


CVector2d CRectangle::GetRightTop() const
{
	return CVector2d(m_horizontalRange.GetMaxValue(), m_verticalRange.GetMinValue()); 
}


void CRectangle::SetTopRight(const CVector2d& topRight)
{
	if (topRight != GetRightTop()){
		istd::CChangeNotifier changeNotifier(this, &s_setTopRightChange);
		Q_UNUSED(changeNotifier);

		m_horizontalRange.SetMaxValue(topRight.GetX());
		m_verticalRange.SetMinValue(topRight.GetY());
	}
}


CVector2d CRectangle::GetLeftBottom() const
{
	return CVector2d(m_horizontalRange.GetMinValue(), m_verticalRange.GetMaxValue()); 
}


void CRectangle::SetBottomLeft(const CVector2d& bottomLeft)
{
	if (bottomLeft != GetLeftBottom()){
		istd::CChangeNotifier changeNotifier(this, &s_setBottomLeftChange);
		Q_UNUSED(changeNotifier);

		m_horizontalRange.SetMinValue(bottomLeft.GetX());
		m_verticalRange.SetMaxValue(bottomLeft.GetY());
	}
}


CVector2d CRectangle::GetRightBottom() const
{
	return CVector2d(m_horizontalRange.GetMaxValue(), m_verticalRange.GetMaxValue()); 
}


void CRectangle::SetBottomRight(const CVector2d& bottomRight)
{
	if (bottomRight != GetRightBottom()){
		istd::CChangeNotifier changeNotifier(this, &s_setBottomRightChange);
		Q_UNUSED(changeNotifier);

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
	istd::CChangeNotifier changeNotifier(this, &s_intersectChange);
	Q_UNUSED(changeNotifier);

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
	istd::CChangeNotifier changeNotifier(this, &s_uniteChange);
	Q_UNUSED(changeNotifier);

	double outputLeft = qMin(rect.GetLeft(), GetLeft());
	double outputTop = qMin(rect.GetTop(), GetTop());
	double outputRight = qMax(rect.GetRight(), GetRight());
	double outputBottom = qMax(rect.GetBottom(), GetBottom());

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
	istd::CChangeNotifier changeNotifier(this, &s_expandChange);
	Q_UNUSED(changeNotifier);

	m_horizontalRange.Expand(rect.m_horizontalRange);
	m_verticalRange.Expand(rect.m_verticalRange);
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
	istd::CChangeNotifier changeNotifier(this, &s_objectMovedChange);
	Q_UNUSED(changeNotifier);

	m_horizontalRange.SetMinValue(m_horizontalRange.GetMinValue() + delta.GetX());
	m_verticalRange.SetMinValue(m_verticalRange.GetMinValue() + delta.GetY());
	m_horizontalRange.SetMaxValue(m_horizontalRange.GetMaxValue() + delta.GetX());
	m_verticalRange.SetMaxValue(m_verticalRange.GetMaxValue() + delta.GetY());
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
		istd::CChangeNotifier changeNotifier(this, &s_objectMovedChange);
		Q_UNUSED(changeNotifier);

		m_verticalRange.SetMinValue(m_verticalRange.GetMinValue() + offset.GetY());
		m_verticalRange.SetMaxValue(m_verticalRange.GetMaxValue() + offset.GetY());

		m_horizontalRange.SetMinValue(m_horizontalRange.GetMinValue() + offset.GetX());
		m_horizontalRange.SetMaxValue(m_horizontalRange.GetMaxValue() + offset.GetX());
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
	i2d::CVector2d leftTop(m_horizontalRange.GetMinValue(), m_verticalRange.GetMinValue());
	i2d::CVector2d rightBottom(m_horizontalRange.GetMaxValue(), m_verticalRange.GetMaxValue());
	i2d::CVector2d leftBottom(m_horizontalRange.GetMinValue(), m_verticalRange.GetMaxValue());
	i2d::CVector2d rightTop(m_horizontalRange.GetMaxValue(), m_verticalRange.GetMinValue());

	i2d::CVector2d transLeftTop, transLeftBottom;
	i2d::CVector2d transRightBottom, transRightTop;

	if (		transformation.GetPositionAt(leftTop, transLeftTop, mode) &&
				transformation.GetPositionAt(rightBottom, transRightBottom, mode) &&
				transformation.GetPositionAt(leftBottom, transLeftBottom, mode) &&
				transformation.GetPositionAt(rightTop, transRightTop, mode)			){
		istd::CChangeNotifier changeNotifier(this, &s_objectModifiedChange);
		Q_UNUSED(changeNotifier);
		
		if (errorFactorPtr != NULL){
			*errorFactorPtr = 0;
		}

		int xmin = qMin(transLeftTop.GetX(), qMin(transLeftBottom.GetX(), qMin(transRightTop.GetX(), transRightBottom.GetX())));
		int xmax = qMax(transLeftTop.GetX(), qMax(transLeftBottom.GetX(), qMax(transRightTop.GetX(), transRightBottom.GetX())));

		m_horizontalRange = istd::CRange(xmin, xmax);
		m_horizontalRange.Validate();

		int ymin = qMin(transLeftTop.GetY(), qMin(transLeftBottom.GetY(), qMin(transRightTop.GetY(), transRightBottom.GetY())));
		int ymax = qMax(transLeftTop.GetY(), qMax(transLeftBottom.GetY(), qMax(transRightTop.GetY(), transRightBottom.GetY())));

		m_verticalRange = istd::CRange(ymin, ymax);
		m_verticalRange.Validate();

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
		istd::CChangeNotifier changeNotifier(this, &s_objectModifiedChange);
		Q_UNUSED(changeNotifier);
		
		if (errorFactorPtr != NULL){
			*errorFactorPtr = 0;
		}

		m_horizontalRange = istd::CRange(transLeftTop.GetX(), transRightBottom.GetX());
		m_verticalRange = istd::CRange(transLeftTop.GetY(), transRightBottom.GetY());

		m_horizontalRange.Validate();
		m_verticalRange.Validate();

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
		*resultRectanglePtr = *this;

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
		*resultRectanglePtr = *this;

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
		istd::CChangeNotifier changeNotifier(this, &s_objectModifiedChange);
		Q_UNUSED(changeNotifier);

		SetHorizontalRange(rectanglePtr->GetHorizontalRange());
		SetVerticalRange(rectanglePtr->GetVerticalRange());

		CObject2dBase::CopyFrom(object, mode);

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
	istd::CChangeNotifier changeNotifier(archive.IsStoring()? NULL: this, &GetAllChanges());
	Q_UNUSED(changeNotifier);

	double& top = m_verticalRange.GetMinValueRef();
	bool retVal = archive.BeginTag(s_topTag);
	retVal = retVal && archive.Process(top);
	retVal = retVal && archive.EndTag(s_topTag);

	double& bottom = m_verticalRange.GetMaxValueRef();
	retVal = retVal && archive.BeginTag(s_bottomTag);
	retVal = retVal && archive.Process(bottom);
	retVal = retVal && archive.EndTag(s_bottomTag);

	double& left = m_horizontalRange.GetMinValueRef();
	retVal = retVal && archive.BeginTag(s_leftTag);
	retVal = retVal && archive.Process(left);
	retVal = retVal && archive.EndTag(s_leftTag);

	double& right = m_horizontalRange.GetMaxValueRef();
	retVal = retVal && archive.BeginTag(s_rightTag);
	retVal = retVal && archive.Process(right);
	retVal = retVal && archive.EndTag(s_rightTag);
	
	return retVal;
}


// static attributes
CRectangle CRectangle::s_empty(0, 0, 0, 0);
CRectangle CRectangle::s_invalid(0, -1, 0, -1);


} // namespace i2d


