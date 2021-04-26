#include <i2d/CPolyline.h>


// ACF includes
#include <istd/TDelPtr.h>
#include <istd/CChangeGroup.h>
#include <istd/CClassInfo.h>


namespace i2d
{


// static constants
static const iser::CArchiveTag s_closedTag("Closed", "Closed", iser::CArchiveTag::TT_LEAF);


// public static methods

QByteArray CPolyline::GetTypeName()
{
	return istd::CClassInfo::GetName<CPolyline>();
}


// public methods

void CPolyline::SetClosed(bool state)
{
	if (m_isClosed != state){
		istd::CChangeNotifier changeNotifier(this);
		Q_UNUSED(changeNotifier);

		m_isClosed = state;
	}
}


int CPolyline::GetSegmentsCount() const
{
	int nodesCount = GetNodesCount();
	if (nodesCount > 0){
		return IsClosed()? nodesCount: nodesCount - 1;
	}

	return 0;
}


i2d::CLine2d CPolyline::GetSegmentLine(int segmentIndex) const
{
	Q_ASSERT(segmentIndex >= 0);
	Q_ASSERT(segmentIndex < GetSegmentsCount());

	int size = GetNodesCount();
	return i2d::CLine2d(GetNodePos(segmentIndex), GetNodePos((segmentIndex + 1) % size));
}


double CPolyline::GetLength() const
{
	if (m_isClosed){
		return BaseClass::GetOutlineLength();
	}

	double length = 0;
	int nodesCount = GetNodesCount();
	if (nodesCount > 0){
		i2d::CLine2d segmentLine;
		segmentLine.SetPoint2(GetNodePos(0));
		for (int nodeIndex = 1; nodeIndex < nodesCount; ++nodeIndex){
			segmentLine.PushEndPoint(GetNodePos(nodeIndex));

			length += segmentLine.GetLength();
		}
	}

	return length;
}


i2d::CVector2d CPolyline::GetKneeVector(int nodeIndex) const
{
	Q_ASSERT(nodeIndex >= 0);

	int nodesCount = GetNodesCount();
	bool isClosed = IsClosed();
	Q_ASSERT(nodeIndex <= nodesCount);

	if (nodesCount <= 1){
		return i2d::CVector2d(0, 0);
	}

	if (!isClosed){
		if (nodeIndex == 0){
			return (GetNodePos(1) - GetNodePos(0)).GetOrthogonal().GetNormalized();
		}
		if (nodeIndex == nodesCount - 1){
			return (GetNodePos(nodeIndex) - GetNodePos(nodeIndex - 1)).GetOrthogonal().GetNormalized();
		}
	}

	const i2d::CVector2d& prevPoint = GetNodePos((nodeIndex + nodesCount - 1) % nodesCount);
	const i2d::CVector2d& point = GetNodePos(nodeIndex);
	const i2d::CVector2d& nextPoint = GetNodePos((nodeIndex + 1) % nodesCount);

	i2d::CVector2d prevNormal = (point - prevPoint).GetOrthogonal().GetNormalized();
	i2d::CVector2d nextNormal = (nextPoint - point).GetOrthogonal().GetNormalized();

	i2d::CVector2d kneeVector;

	double factor = prevNormal.GetDotProduct(nextNormal) + 1;

	if (factor > I_BIG_EPSILON){
		kneeVector = (prevNormal + nextNormal) / factor;
	}
	else{
		kneeVector = i2d::CVector2d(1, 0);
	}

	return kneeVector;
}

bool CPolyline::GetAdjacentNodes(double atPositionNormalized, i2d::CVector2d& previous, i2d::CVector2d& next, double& alpha) const
{
	int previousIndex, nextIndex;
	if (GetAdjacentNodeIndices(atPositionNormalized, previousIndex, nextIndex, alpha)){
		previous = GetNodePos(previousIndex);
		next = GetNodePos(nextIndex);
		return true;
	}

	return false;
}


bool CPolyline::GetAdjacentNodes(const i2d::CVector2d& position, i2d::CVector2d& previous, i2d::CVector2d& next) const
{
	int previousIndex, nextIndex;
	if (GetAdjacentNodeIndices(position, previousIndex, nextIndex)){
		previous = GetNodePos(previousIndex);
		next = GetNodePos(nextIndex);
		return true;
	}

	return false;
}


bool CPolyline::GetAdjacentNodeIndices(double atPositionNormalized, int& previousIndex, int& nextIndex, double& alpha) const
{
	Q_ASSERT(atPositionNormalized >= 0);
	Q_ASSERT(atPositionNormalized <= 1);

	const int nodesCount = GetNodesCount();

	double positionOnLine = atPositionNormalized * GetLength();

	nextIndex = 0;
	previousIndex = 0;

	istd::CRange lengthRange(0, 0);
	i2d::CVector2d prevNodePos, nextNodePos;

	while (positionOnLine > lengthRange.GetMaxValue() - I_BIG_EPSILON){
		previousIndex = nextIndex;
		prevNodePos = GetNodePos(previousIndex);

		nextIndex = (nextIndex + 1) % nodesCount;

		nextNodePos = GetNodePos(nextIndex);
		double lengthMax = lengthRange.GetMaxValue() + nextNodePos.GetDistance(prevNodePos);

		lengthRange.SetMinValue(lengthRange.GetMaxValue());
		lengthRange.SetMaxValue(lengthMax);
	}

	alpha = (positionOnLine - lengthRange.GetMinValue()) / lengthRange.GetLength();

	if (alpha < 0){
		return false;
	}

	return true;
}


bool CPolyline::GetAdjacentNodeIndices(const i2d::CVector2d& point, int& previousIndex, int& nextIndex) const
{
	int nodesCount = GetNodesCount();
	int segmentsCount = GetSegmentsCount();

	nextIndex = 0;
	previousIndex = 0;

	bool retVal = false;
	double minDistance = qInf();

	if (segmentsCount > 0){
		Q_ASSERT(nodesCount > 0);	// No segments without nodes!

		int prevNodeIndex = 0;
		i2d::CVector2d prevNodePos = GetNodePos(prevNodeIndex);

		i2d::CVector2d prevKnee = GetKneeVector(prevNodeIndex);
		i2d::CVector2d prevToPoint = point - prevNodePos;
		bool afterPrev = (prevKnee.GetCrossProductZ(prevToPoint) > 0);

		for (int segmentIndex = 0; segmentIndex < segmentsCount; ++segmentIndex){
			int nextNodeIndex = (segmentIndex + 1) % nodesCount;
			i2d::CVector2d nextNodePos = GetNodePos(nextNodeIndex);
			i2d::CVector2d nextKnee = GetKneeVector(nextNodeIndex);

			i2d::CVector2d nextToPoint = point - nextNodePos;

			bool afterNext = (nextKnee.GetCrossProductZ(nextToPoint) > 0);
			if (afterPrev && !afterNext){
				const i2d::CLine2d segment = GetSegmentLine(segmentIndex);
				const double totalDistance = segment.GetDistance(point);
				if (totalDistance < minDistance){
					previousIndex = prevNodeIndex;
					nextIndex = nextNodeIndex;
					retVal = true;

					minDistance = totalDistance;
				}
			}

			prevNodeIndex = nextNodeIndex;
			prevNodePos = nextNodePos;
			afterPrev = afterNext;
		}

		if (!retVal){
			previousIndex = prevNodeIndex - 1;
			nextIndex = segmentsCount % nodesCount;
			retVal = true;
		}
	}

	return retVal;
}

bool CPolyline::GetAdjacentLine(double atPositionNormalized, i2d::CLine2d& line, double& alpha) const
{
	i2d::CVector2d a;
	i2d::CVector2d b;
	if (GetAdjacentNodes(atPositionNormalized, a, b, alpha)){
		line = i2d::CLine2d(a, b);
		line.SetCalibration(GetCalibration());
		return true;
	}

	return false;
}


bool CPolyline::GetAdjacentLine(const i2d::CVector2d& position, i2d::CLine2d& line) const
{
	i2d::CVector2d a;
	i2d::CVector2d b;
	if (GetAdjacentNodes(position, a, b)){
		line = i2d::CLine2d(a, b);
		line.SetCalibration(GetCalibration());
		return true;
	}

	return false;
}

bool CPolyline::GetInterpolatedPosition(const double atPositionNormalized, i2d::CVector2d& output) const
{
	i2d::CLine2d adhesiveAoiLineSegment;
	double alpha;
	if (GetAdjacentLine(atPositionNormalized, adhesiveAoiLineSegment, alpha)){
		output = adhesiveAoiLineSegment.GetPoint1() * (1.0 - alpha) + adhesiveAoiLineSegment.GetPoint2() * alpha;
		return true;
	}

	return false;
}

i2d::CVector2d CPolyline::GetInterpolatedPosition(double position) const
{
	i2d::CVector2d result = i2d::CVector2d::GetZero();
	GetInterpolatedPosition(position, result);

	return result;
}


// reimplemented (iser::ISerializable)

bool CPolyline::Serialize(iser::IArchive& archive)
{
	quint32 version = quint32(-1);
	archive.GetVersionInfo().GetVersionNumber(iser::IVersionInfo::AcfVersionId, version);

	bool retVal = true;
	if (version >= 4052){
		retVal = retVal && archive.BeginTag(s_closedTag);
		retVal = retVal && archive.Process(m_isClosed);
		retVal = retVal && archive.EndTag(s_closedTag);
	}

	retVal = retVal && BaseClass::Serialize(archive);

	if ((version > 3930) && (version < 4052)){
		retVal = retVal && archive.BeginTag(s_closedTag);
		retVal = retVal && archive.Process(m_isClosed);
		retVal = retVal && archive.EndTag(s_closedTag);
	}

	return retVal;
}


// reimplemented (iser::IObject)

QByteArray CPolyline::GetFactoryId() const {

	return GetTypeName();
}


// reimplemented (istd::IChangeable)

bool CPolyline::CopyFrom(const IChangeable& object, CompatibilityMode mode)
{
	istd::CChangeGroup changeGroup(this);
	Q_UNUSED(changeGroup);

	if (BaseClass::CopyFrom(object, mode)){
		const CPolyline* polylinePtr = dynamic_cast<const CPolyline*>(&object);
		if (polylinePtr != NULL){
			SetClosed(polylinePtr->IsClosed());
		}
		else{
			SetClosed(true);
		}

		return true;
	}

	return false;
}


istd::IChangeable* CPolyline::CloneMe(CompatibilityMode mode) const 
{
	istd::TDelPtr<CPolyline> clonePtr(new CPolyline);

	if (clonePtr->CopyFrom(*this, mode)){
		return clonePtr.PopPtr();
	}

	return NULL;
}


bool CPolyline::IsEqual(const IChangeable& object) const
{
	const CPolyline* polylinePtr = dynamic_cast<const CPolyline*>(&object);
	if (polylinePtr != NULL){
		return (m_isClosed == polylinePtr->m_isClosed) && BaseClass::IsEqual(object);	
	}

	return false;
}


} // namespace i2d

