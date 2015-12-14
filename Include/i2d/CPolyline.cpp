#include "i2d/CPolyline.h"


// ACF includes
#include "istd/TDelPtr.h"
#include "istd/CChangeNotifier.h"


namespace i2d
{


// static constants
static const iser::CArchiveTag s_closedTag("Closed", "Closed", iser::CArchiveTag::TT_LEAF);


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
	return i2d::CLine2d(GetNode(segmentIndex), GetNode((segmentIndex + 1) % size));
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
		segmentLine.SetPoint2(GetNode(0));
		for (int nodeIndex = 1; nodeIndex < nodesCount; ++nodeIndex){
			segmentLine.PushEndPoint(GetNode(nodeIndex));

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
			return (GetNode(1) - GetNode(0)).GetOrthogonal().GetNormalized();
		}
		if (nodeIndex == nodesCount - 1){
			return (GetNode(nodeIndex) - GetNode(nodeIndex - 1)).GetOrthogonal().GetNormalized();
		}
	}

	const i2d::CVector2d& prevPoint = GetNode((nodeIndex + nodesCount - 1) % nodesCount);
	const i2d::CVector2d& point = GetNode(nodeIndex);
	const i2d::CVector2d& nextPoint = GetNode((nodeIndex + 1) % nodesCount);

	i2d::CVector2d prevNormal = (point - prevPoint).GetOrthogonal().GetNormalized();
	i2d::CVector2d nextNormal = (nextPoint - point).GetOrthogonal().GetNormalized();

	i2d::CVector2d kneeVector;

	double dotProduct = prevNormal.GetDotProduct(nextNormal);

	if (dotProduct > -1.0){
		kneeVector = (prevNormal + nextNormal) / (1 + dotProduct);
	}
	else{
		kneeVector = i2d::CVector2d(1, 0);
	}

	return kneeVector;
}


// reimplemented (iser::ISerializable)

bool CPolyline::Serialize(iser::IArchive& archive)
{
	quint32 version = quint32(-1);
	archive.GetVersionInfo().GetVersionNumber(iser::IVersionInfo::AcfVersionId, version);

	bool retVal = true;
	if (version >= 4051){
		retVal = retVal && archive.BeginTag(s_closedTag);
		retVal = retVal && archive.Process(m_isClosed);
		retVal = retVal && archive.EndTag(s_closedTag);
	}

	retVal = retVal && BaseClass::Serialize(archive);

	if ((version > 3930) && (version < 4051)){
		retVal = retVal && archive.BeginTag(s_closedTag);
		retVal = retVal && archive.Process(m_isClosed);
		retVal = retVal && archive.EndTag(s_closedTag);
	}

	return retVal;
}


// reimplemented (istd::IChangeable)

bool CPolyline::CopyFrom(const IChangeable& object, CompatibilityMode mode)
{
	const CPolyline* polylinePtr = dynamic_cast<const CPolyline*>(&object);

	if (polylinePtr != NULL){
		istd::CChangeNotifier changeNotifier(this);
		Q_UNUSED(changeNotifier);

		BaseClass::CopyFrom(object, mode);

		SetClosed(polylinePtr->IsClosed());

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

