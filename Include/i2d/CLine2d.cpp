#include "i2d/CLine2d.h"


// ACF includes
#include "istd/CChangeNotifier.h"
#include "istd/TDelPtr.h"
#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"

#include "i2d/CRectangle.h"


namespace i2d
{


// public members

CLine2d::CLine2d()
:	m_point1(0, 0),
	m_point2(0, 0)
{
}


CLine2d::CLine2d(const CVector2d& point1, const CVector2d& point2)
:	m_point1(point1),
	m_point2(point2)
{
}


CLine2d::CLine2d(double x1, double y1, double x2, double y2)
	:m_point1(x1,y1),
	m_point2(x2,y2)
{
}


void CLine2d::SetPoint1(const CVector2d& point)
{
	if (point != m_point1){
		BeginChanges(GetAnyChange());

		m_point1 = point;

		EndChanges(GetAnyChange());
	}
}


void CLine2d::SetPoint2(const CVector2d& point)
{
	if (point != m_point2){
		BeginChanges(GetAnyChange());

		m_point2 = point;

		EndChanges(GetAnyChange());
	}
}


double CLine2d::GetSlope() const
{
	double y1 = m_point1.GetY();
	double y2 = m_point2.GetY();
	double x1 = m_point1.GetX();
	double x2 = m_point2.GetX();

	if (x2 == x1){
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

	if (x2 == x1){
		return I_INFINITY;
	}

	return -(-x2 * y1 + y2 * x1) / (x2 - x1);
}


bool CLine2d::IsParallel(const CLine2d& line) const
{
	CVector2d diff = GetDiffVector();
	CVector2d lineDiff = line.GetDiffVector();

	double scale = qAbs(diff.GetCrossProductZ(lineDiff));

	return (qAbs(scale) < I_EPSILON);
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

	double scale = qAbs(diff.GetCrossProductZ(lineDiff));
	if (qAbs(scale) > I_EPSILON){
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

	double scale = qAbs(diff.GetCrossProductZ(lineDiff));
	if (qAbs(scale) < I_EPSILON){
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

	return qAbs(diff.GetCrossProductZ(point1Diff) / diffLength);	// return distance to the line
}


double CLine2d::GetMaxDistance(const CVector2d& position) const
{
	double point1Dist2 = m_point1.GetDistance2(position);
	double point2Dist2 = m_point2.GetDistance2(position);

	if (point1Dist2 > point2Dist2){
		return qSqrt(point1Dist2);
	}
	else{
		return qSqrt(point2Dist2);
	}
}


double CLine2d::GetExtendedDistance(const CVector2d& position) const
{
	CVector2d diff = GetDiffVector();
	double diffLength = diff.GetLength();

	CVector2d point1Diff = position - m_point1;

	return qAbs(diff.GetCrossProductZ(point1Diff) / diffLength);	// return distance to the line
}


CVector2d CLine2d::GetPositionFromAlpha(double alpha) const
{
	return m_point1 * (1 - alpha) + m_point2 * alpha;
}


double CLine2d::GetLength() const
{
	return GetDiffVector().GetLength();
}


double CLine2d::GetLength2() const
{
	return GetDiffVector().GetLength2();
}


double CLine2d::GetDirectionAngle() const
{
	return GetDiffVector().GetAngle();
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


void CLine2d::PushBeginPoint(const i2d::CVector2d& newBeginPoint)
{
	BeginChangeGroup(GetNoChanges());

	SetPoint2(GetPoint1());
	SetPoint1(newBeginPoint);

	EndChangeGroup(GetNoChanges());
}


void CLine2d::PushEndPoint(const i2d::CVector2d& newEndPoint)
{
	BeginChangeGroup(GetNoChanges());

	SetPoint1(GetPoint2());
	SetPoint2(newEndPoint);

	EndChangeGroup(GetNoChanges());
}


bool CLine2d::GetCutPoint(const CLine2d& otherLine, i2d::CVector2d& cutPoint) const
{
	if (!IsIntersectedBy(otherLine)){
		return false;
	}

	double x1 = otherLine.GetPoint1().GetX();
	double x2 = otherLine.GetPoint2().GetX();

	double y1 = otherLine.GetPoint1().GetY();
	double y2 = otherLine.GetPoint2().GetY();

	double x3 = GetPoint1().GetX();
	double x4 = GetPoint2().GetX();

	double y3 = GetPoint1().GetY();
	double y4 = GetPoint2().GetY();

	if (x1 == x2 && y3 == y4){
		cutPoint = i2d::CVector2d(x1,y3);
		return true;
	}

	if (y1 == y2 && x3 == x4){
		cutPoint = i2d::CVector2d(x3,y1);
		return true;
	}

	double dx1 = x2 - x1;
	if (qAbs(dx1) < I_BIG_EPSILON){
		dx1 = I_BIG_EPSILON;
	}

	double dx2 = x4 - x3;
	if (qAbs(dx2) < I_BIG_EPSILON){
		dx2 = I_BIG_EPSILON;
	}

	double m1 = (y2 - y1) / dx1;
	double m2 = (y4 - y3) / dx2;

	// no intersection was found:
	if ( qAbs(m2 - m1) < I_BIG_EPSILON){
		return false;
	}

	double b1 = -(-x2 * y1 + y2 * x1) / dx1;
	double b2 = -(-x4 * y3 + y4 * x3) / dx2;

	double x = (b2 - b1) / (m1 - m2);
	double y = m1 * x + b1;

	cutPoint = i2d::CVector2d(x,y);
	return true;
}



i2d::CVector2d CLine2d::GetNearestPoint(const i2d::CVector2d& point) const
{
	double alpha = GetCastAlpha(point);

	if (alpha <= 0){
		return m_point1;
	}
	else if (alpha >= 1){
		return m_point2;
	}
	else{
		return GetPositionFromAlpha(alpha);
	}
}


i2d::CVector2d CLine2d::GetExtendedNearestPoint(const i2d::CVector2d& point) const
{
	double x1 = m_point1.GetX();		double y1 = m_point1.GetY();
	double x2 = m_point2.GetX();		double y2 = m_point2.GetY();
	double x3 = point.GetX();			double y3 = point.GetY();

	double x = ((x1 - x2) * x3 / (y2 - y1) - (y2 - y1) * x1 / (x2 - x1) + y1 - y3 ) /
		((x1 - x2) / (y2 - y1) - (y2 - y1) / (x2 - x1));

	double y = (x1 - x2) * x / (y2 - y1) - (x1 - x2) * x3 / (y2 - y1) + y3;

	return i2d::CVector2d(x, y);
}


CLine2d CLine2d::GetShortestEndConnection(const CLine2d& line) const
{
	double dist1 = m_point1.GetDistance(line.m_point1);
	double dist2 = m_point2.GetDistance(line.m_point1);
	double dist3 = m_point1.GetDistance(line.m_point2);
	double dist4 = m_point2.GetDistance(line.m_point2);
	if ((dist1 < dist2) && (dist1 < dist3) && (dist1 < dist4)){
		return CLine2d(m_point1, line.m_point1);
	}
	else if ((dist2 < dist3) && (dist2 < dist4)){
		return CLine2d(m_point2, line.m_point1);
	}
	else if (dist3 < dist4){
		return CLine2d(m_point1, line.m_point2);
	}
	else{
		return CLine2d(m_point2, line.m_point2);
	}
}


CLine2d CLine2d::GetShortestConnectionToNext(const CLine2d& line) const
{	// TODO: implement it better
	double dist1 = line.GetDistance(m_point2);
	double dist2 = GetDistance(line.m_point1);
	if (dist1 < dist2){
		return CLine2d(m_point2, line.GetNearestPoint(m_point2));
	}
	else{
		return CLine2d(GetNearestPoint(line.m_point1), line.m_point1);
	}
}


CLine2d CLine2d::GetShortestConnection(const i2d::CVector2d& point) const
{
	double alpha = GetCastAlpha(point);

	if (alpha <= 0){
		return CLine2d(m_point1, point);
	}
	else if (alpha >= 1){
		return CLine2d(m_point2, point);
	}
	else{
		return CLine2d(GetPositionFromAlpha(alpha), point);
	}
}


CLine2d CLine2d::GetShortestConnection(const CLine2d& line) const
{	// TODO: implement it better
	double dist1 = line.GetDistance(m_point1);
	double dist2 = line.GetDistance(m_point2);
	double dist3 = GetDistance(line.m_point1);
	double dist4 = GetDistance(line.m_point2);
	if ((dist1 < dist2) && (dist1 < dist3) && (dist1 < dist4)){
		return CLine2d(m_point1, line.GetNearestPoint(m_point1));
	}
	else if ((dist2 < dist3) && (dist2 < dist4)){
		return CLine2d(m_point2, line.GetNearestPoint(m_point2));
	}
	else if (dist3 < dist4){
		return CLine2d(GetNearestPoint(line.m_point1), line.m_point1);
	}
	else{
		return CLine2d(GetNearestPoint(line.m_point2), line.m_point2);
	}
}


// reimplemented (i2d::IObject2d)

CVector2d CLine2d::GetCenter() const
{
	return GetBoundingBox().GetCenter();
}


void CLine2d::MoveCenterTo(const CVector2d& position)
{
	i2d::CVector2d offset = position - GetCenter();

	if (offset != i2d::CVector2d(0, 0)){
		BeginChanges(s_objectPositionChangeSet);

		SetPoint1(GetPoint1() + offset);
		SetPoint2(GetPoint2() + offset);

		EndChanges(s_objectPositionChangeSet);
	}
}


CRectangle CLine2d::GetBoundingBox() const
{
	double left = qMin(m_point1.GetX(), m_point2.GetX());
	double top = qMin(m_point1.GetY(), m_point2.GetY());
	double right = qMax(m_point1.GetX(), m_point2.GetX());
	double bottom = qMax(m_point1.GetY(), m_point2.GetY());

	return CRectangle(left, top, right - left, bottom - top);
}


bool CLine2d::Transform(
			const ITransformation2d& transformation,
			ITransformation2d::ExactnessMode mode,
			double* errorFactorPtr)
{
	CVector2d transPos1;
	CVector2d transPos2;
	if (!transformation.GetPositionAt(m_point1, transPos1, mode) || !transformation.GetPositionAt(m_point2, transPos2, mode)){
		return false;
	}

	if (errorFactorPtr != NULL){
		if ((transformation.GetTransformationFlags() & ITransformation2d::TF_AFFINE) != 0){
			*errorFactorPtr = 0;
		}
		else{
			// approx error if transformation of center lies on the result line
			CVector2d transCenter;
			if (transformation.GetPositionAt(GetCenter(), transCenter, mode)){
				*errorFactorPtr = qSqrt((transCenter - transPos1).GetCrossProductZ(transPos2 - transPos1));
			}
			else{
				// there is no possibility to calculate it, get proportional dummy error
				*errorFactorPtr = transPos2.GetDistance(transPos1);
			}
		}
	}

    BeginChanges(s_objectPositionChangeSet);

	m_point1 = transPos1;
	m_point2 = transPos2;

    EndChanges(s_objectPositionChangeSet);

	return true;
}


bool CLine2d::InvTransform(
			const ITransformation2d& transformation,
			ITransformation2d::ExactnessMode mode,
			double* errorFactorPtr)
{
	CVector2d transPos1;
	CVector2d transPos2;
	if (!transformation.GetInvPositionAt(m_point1, transPos1, mode) || !transformation.GetInvPositionAt(m_point2, transPos2, mode)){
		return false;
	}

	if (errorFactorPtr != NULL){
		if ((transformation.GetTransformationFlags() & ITransformation2d::TF_AFFINE) != 0){
			*errorFactorPtr = 0;
		}
		else{
			// approx error if transformation of center lies on the result line
			CVector2d transCenter;
			if (transformation.GetInvPositionAt(GetCenter(), transCenter, mode)){
				*errorFactorPtr = qSqrt((transCenter - transPos1).GetCrossProductZ(transPos2 - transPos1));
			}
			else{
				// there is no possibility to calculate it, get proportional dummy error
				*errorFactorPtr = transPos2.GetDistance(transPos1);
			}
		}
	}

	BeginChanges(s_objectPositionChangeSet);

	m_point1 = transPos1;
	m_point2 = transPos2;

	EndChanges(s_objectPositionChangeSet);

	return true;
}


bool CLine2d::GetTransformed(
			const ITransformation2d& transformation,
			IObject2d& result,
			ITransformation2d::ExactnessMode mode,
			double* errorFactorPtr) const
{
	CLine2d* resultLinePtr = dynamic_cast<CLine2d*>(&result);
	if (resultLinePtr == NULL){
		return false;
	}

	CVector2d transPos1;
	CVector2d transPos2;
	if (!transformation.GetPositionAt(m_point1, transPos1, mode) || !transformation.GetPositionAt(m_point2, transPos2, mode)){
		return false;
	}

	if (errorFactorPtr != NULL){
		if ((transformation.GetTransformationFlags() & ITransformation2d::TF_AFFINE) != 0){
			*errorFactorPtr = 0;
		}
		else{
			// approx error if transformation of center lies on the result line
			CVector2d transCenter;
			if (transformation.GetPositionAt(GetCenter(), transCenter, mode)){
				*errorFactorPtr = qSqrt((transCenter - transPos1).GetCrossProductZ(transPos2 - transPos1));
			}
			else{
				// there is no possibility to calculate it, get proportional dummy error
				*errorFactorPtr = transPos2.GetDistance(transPos1);
			}
		}
	}

	resultLinePtr->BeginChanges(s_objectPositionChangeSet);

	resultLinePtr->SetPoint1(transPos1);
	resultLinePtr->SetPoint2(transPos2);

	resultLinePtr->EndChanges(s_objectPositionChangeSet);

	return true;
}


bool CLine2d::GetInvTransformed(
			const ITransformation2d& transformation,
			IObject2d& result,
			ITransformation2d::ExactnessMode mode,
			double* errorFactorPtr) const
{
	CLine2d* resultLinePtr = dynamic_cast<CLine2d*>(&result);
	if (resultLinePtr == NULL){
		return false;
	}

	CVector2d transPos1;
	CVector2d transPos2;
	if (!transformation.GetInvPositionAt(m_point1, transPos1, mode) || !transformation.GetInvPositionAt(m_point2, transPos2, mode)){
		return false;
	}

	if (errorFactorPtr != NULL){
		if ((transformation.GetTransformationFlags() & ITransformation2d::TF_AFFINE) != 0){
			*errorFactorPtr = 0;
		}
		else{
			// approx error if transformation of center lies on the result line
			CVector2d transCenter;
			if (transformation.GetInvPositionAt(GetCenter(), transCenter, mode)){
				*errorFactorPtr = qSqrt((transCenter - transPos1).GetCrossProductZ(transPos2 - transPos1));
			}
			else{
				// there is no possibility to calculate it, get proportional dummy error
				*errorFactorPtr = transPos2.GetDistance(transPos1);
			}
		}
	}

	resultLinePtr->BeginChanges(s_objectPositionChangeSet);

	resultLinePtr->SetPoint1(transPos1);
	resultLinePtr->SetPoint2(transPos2);

    resultLinePtr->EndChanges(s_objectPositionChangeSet);

	return true;
}


// reimplemented (istd::IChangeable)

int CLine2d::GetSupportedOperations() const
{
	return SO_COPY | SO_CLONE;
}


bool CLine2d::CopyFrom(const IChangeable& object, CompatibilityMode mode)
{
	const CLine2d* line2dPtr = dynamic_cast<const CLine2d*>(&object);

	if (line2dPtr != NULL){
		BeginChanges(s_objectPositionChangeSet);
		
		SetPoint1(line2dPtr->GetPoint1());
		SetPoint2(line2dPtr->GetPoint2());

		CObject2dBase::CopyFrom(object, mode);

		EndChanges(s_objectPositionChangeSet);

		return true;
	}

	return false;
}


istd::IChangeable* CLine2d::CloneMe(CompatibilityMode mode) const 
{
	istd::TDelPtr<CLine2d> clonePtr(new CLine2d);

	if (clonePtr->CopyFrom(*this, mode)){
		return clonePtr.PopPtr();
	}

	return NULL;
}


// reimplemented (iser::ISerializable)

bool CLine2d::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag beginPointTag("BeginPoint", "First point of line", iser::CArchiveTag::TT_GROUP);
	static iser::CArchiveTag endPointTag("EndPoint", "Second point of line", iser::CArchiveTag::TT_GROUP);

	istd::CChangeNotifier notifier(archive.IsStoring()? NULL: this, &GetAllChanges());
	Q_UNUSED(notifier);

	bool retVal = archive.BeginTag(beginPointTag);
	retVal = retVal && m_point1.Serialize(archive);
	retVal = retVal && archive.EndTag(beginPointTag);

	retVal = retVal && archive.BeginTag(endPointTag);
	retVal = retVal && m_point2.Serialize(archive);
	retVal = retVal && archive.EndTag(endPointTag);

	return retVal;
}


} // namespace i2d


