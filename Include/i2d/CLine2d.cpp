#include "i2d/CLine2d.h"


#include "istd/TChangeNotifier.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"

#include "i2d/CRectangle.h"


namespace i2d
{


CLine2d::CLine2d()
	:m_point1(0, 0),
	m_point2(0, 0)
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


void CLine2d::SetPoint1(const CVector2d& point)
{
	if (point != m_point1){
		istd::CChangeNotifier notifier(this);

		m_point1 = point;
	}
}


void CLine2d::SetPoint2(const CVector2d& point)
{
	if (point != m_point2){
		istd::CChangeNotifier notifier(this);

		m_point2 = point;
	}
}


CLine2d CLine2d::operator=(const CLine2d& inl)
{
	m_point1 = inl.m_point1;
	m_point2 = inl.m_point2;

	return *this;
}


double CLine2d::GetSlope() const
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


double CLine2d::GetIntercept() const
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
	CVector2d diff = GetDiffVector();
	CVector2d lineDiff = line.GetDiffVector();

	double scale = fabs(diff.GetCrossProductZ(lineDiff));

	return (fabs(scale) < I_EPSILON);
}


bool CLine2d::IsIntersectedBy(const CLine2d& line) const
{
	CVector2d diff = GetDiffVector();

	if ((diff.GetCrossProductZ(line.m_point1 - m_point1) > 0) == (diff.GetCrossProductZ(line.m_point2 - m_point1) > 0)){
		return false;	// both points of second line lie on the same side of the first one
	}

	CVector2d lineDiff = line.GetDiffVector();

	if ((lineDiff.GetCrossProductZ(m_point1 - line.m_point1) > 0) == (lineDiff.GetCrossProductZ(m_point2 - line.m_point1) > 0)){
		return false;	// both points of first line lie on the same side of the second one
	}

	return true;
}


bool CLine2d::GetIntersection(const CLine2d& line, CVector2d& result) const
{
	CVector2d diff = GetDiffVector();

	if ((diff.GetCrossProductZ(line.m_point1 - m_point1) > 0) == (diff.GetCrossProductZ(line.m_point2 - m_point1) > 0)){
		return false;	// both points of second line lie on the same side of the first one
	}

	CVector2d lineDiff = line.GetDiffVector();

	if ((lineDiff.GetCrossProductZ(m_point1 - line.m_point1) > 0) == (lineDiff.GetCrossProductZ(m_point2 - line.m_point1) > 0)){
		return false;	// both points of first line lie on the same side of the second one
	}

	double scale = fabs(diff.GetCrossProductZ(lineDiff));
	if (fabs(scale) > I_EPSILON){
		CVector2d point1Diff = line.m_point1 - m_point1;

		result = m_point1 + diff * ((point1Diff.GetCrossProductZ(lineDiff) * diff.GetCrossProductZ(lineDiff)) / (scale * scale));
	}
	else{
		result = (m_point1 + m_point2 + line.m_point1 + line.m_point2) * 0.25;
	}

	return true;
}


bool CLine2d::GetExtendedIntersection(const CLine2d& line, CVector2d& result) const
{
	CVector2d diff = GetDiffVector();
	CVector2d lineDiff = line.GetDiffVector();

	double scale = fabs(diff.GetCrossProductZ(lineDiff));
	if (fabs(scale) < I_EPSILON){
		return false;
	}

	CVector2d point1Diff = line.m_point1 - m_point1;

	result = m_point1 + diff * ((point1Diff.GetCrossProductZ(lineDiff) * diff.GetCrossProductZ(lineDiff)) / (scale * scale));

	return true;
}


double CLine2d::GetDistance(const CVector2d& position) const
{
	CVector2d diff = GetDiffVector();
	double diffLength = diff.GetLength();

	CVector2d point1Diff = position - m_point1;
	double dotProduct = point1Diff.GetDotProduct(diff);
	if (dotProduct < 0){	// if the first point is the nearest
		return m_point1.GetDistance(position);
	}
	if (dotProduct > diffLength * diffLength){	// if the second point is the nearest
		return m_point2.GetDistance(position);
	}

	return fabs(diff.GetCrossProductZ(point1Diff) / diffLength);	// return distance to the line
}


double CLine2d::GetExtendedDistance(const CVector2d& position) const
{
	CVector2d diff = GetDiffVector();
	double diffLength = diff.GetLength();

	CVector2d point1Diff = position - m_point1;

	return fabs(diff.GetCrossProductZ(point1Diff) / diffLength);	// return distance to the line
}


CVector2d CLine2d::GetPositionFromAlpha(double alpha) const
{
	return m_point1 * (1 - alpha) + m_point2 * alpha;
}


CLine2d CLine2d::GetClipped(const CRectangle& rect) const
{
	static CLine2d emptyLine(CVector2d(0, 0), CVector2d(0, 0));

	CLine2d retVal = *this;

	bool isLeftOut1 = (retVal.m_point1.GetX() < rect.GetLeft());
	bool isLeftOut2 = (retVal.m_point2.GetX() < rect.GetLeft());

	if (isLeftOut1 != isLeftOut2){
		CVector2d& cutPoint = isLeftOut1? retVal.m_point1: retVal.m_point2;

		cutPoint.SetX(rect.GetLeft());
		cutPoint.SetY(GetCutYPos(rect.GetLeft()));
	}
	else if (isLeftOut1 && isLeftOut2){
		return emptyLine;
	}

	bool isTopOut1 = (retVal.m_point1.GetY() < rect.GetTop());
	bool isTopOut2 = (retVal.m_point2.GetY() < rect.GetTop());

	if (isTopOut1 != isTopOut2){
		CVector2d& cutPoint = isTopOut1? retVal.m_point1: retVal.m_point2;

		cutPoint.SetX(GetCutXPos(rect.GetTop()));
		cutPoint.SetY(rect.GetTop());
	}
	else if (isTopOut1 && isTopOut2){
		return emptyLine;
	}

	bool isRightOut1 = (retVal.m_point1.GetX() > rect.GetRight());
	bool isRightOut2 = (retVal.m_point2.GetX() > rect.GetRight());

	if (isRightOut1 != isRightOut2){
		CVector2d& cutPoint = isRightOut1? retVal.m_point1: retVal.m_point2;

		cutPoint.SetX(rect.GetRight());
		cutPoint.SetY(GetCutYPos(rect.GetRight()));
	}
	else if (isRightOut1 && isRightOut2){
		return emptyLine;
	}

	bool isBottomOut1 = (retVal.m_point1.GetY() > rect.GetBottom());
	bool isBottomOut2 = (retVal.m_point2.GetY() > rect.GetBottom());

	if (isBottomOut1 != isBottomOut2){
		CVector2d& cutPoint = isBottomOut1? retVal.m_point1: retVal.m_point2;

		cutPoint.SetX(GetCutXPos(rect.GetBottom()));
		cutPoint.SetY(rect.GetBottom());
	}
	else if (isBottomOut1 && isBottomOut2){
		return emptyLine;
	}

	return retVal;
}


CLine2d CLine2d::GetSwapped() const
{
	return i2d::CLine2d(m_point2, m_point1);
}


CRectangle CLine2d::GetBoundingBox() const
{
	double left = istd::Min(m_point1.GetX(), m_point2.GetX());
	double top = istd::Min(m_point1.GetY(), m_point2.GetY());
	double right = istd::Max(m_point1.GetX(), m_point2.GetX());
	double bottom = istd::Max(m_point1.GetY(), m_point2.GetY());

	return CRectangle(left, top, right - left, bottom - top);
}


// reimplemented (IObject2d)

CVector2d CLine2d::GetCenter() const
{
	return GetBoundingBox().GetCenter();
}


void CLine2d::MoveTo(const CVector2d& position)
{
	i2d::CVector2d offset = position - GetCenter();

	SetPoint1(GetPoint1() + offset);
	SetPoint2(GetPoint2() + offset);
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

