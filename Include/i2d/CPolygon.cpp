#include "i2d/CPolygon.h"


// ACF includes
#include "i2d/CLine2d.h"

#include "istd/TChangeNotifier.h"
#include "istd/TDelPtr.h"


namespace i2d
{


// public methods

void CPolygon::Clear()
{
	m_nodes.clear();
}


void CPolygon::SetNodesCount(int nodesCount)
{
	m_nodes.resize(nodesCount);
}


bool CPolygon::InsertNode(const i2d::CVector2d& node)
{
	m_nodes.push_back(node);

	return true;
}


bool CPolygon::InsertNode(int index, const i2d::CVector2d& node)
{
	QVector<i2d::CVector2d>::iterator iter = m_nodes.begin();
	iter += index;
	m_nodes.insert(iter, node);

	return true;
}


bool CPolygon::RemoveNode(int index)
{
	QVector<i2d::CVector2d>::iterator iter = m_nodes.begin();
	iter += index;
	m_nodes.erase(iter);

	return true;
}


double CPolygon::GetOutlineLength() const
{
	double length = 0;
	int nodesCount = GetNodesCount();
	if (nodesCount > 0){
		i2d::CLine2d segmentLine;
		segmentLine.SetPoint2(GetNode(nodesCount - 1));
		for (int nodeIndex = 0; nodeIndex < nodesCount; ++nodeIndex){
			segmentLine.PushEndPoint(GetNode(nodeIndex));
			length += segmentLine.GetLength();
		}
	}
	return length;
}


// reimplemented (i2d::IObject2d)

CVector2d CPolygon::GetCenter() const
{
	return GetBoundingBox().GetCenter();
}


void CPolygon::MoveCenterTo(const CVector2d& position)
{
	i2d::CVector2d offset = position - GetCenter();
	if (offset != i2d::CVector2d(0, 0)){
		istd::CChangeNotifier notifier(this, CF_OBJECT_POSITION | CF_MODEL);

		int nodesCount = GetNodesCount();
		for (int i = 0; i < nodesCount; i++){
			SetNode(i, GetNode(i) + offset);
		}
	}
}


i2d::CRectangle CPolygon::GetBoundingBox() const
{
	int nodesCount = GetNodesCount();

	if (nodesCount > 0){
		const i2d::CVector2d& firstPoint = GetNode(0);
		i2d::CRectangle boundingBox(firstPoint, firstPoint);
		for (int i = 1; i < nodesCount; i++){
			const i2d::CVector2d& point = GetNode(i);

			boundingBox.Unite(point);
		}

		return boundingBox;
	}

	return i2d::CRectangle::GetEmpty();
}


bool CPolygon::Transform(
			const ITransformation2d& transformation,
			ITransformation2d::ExactnessMode mode,
			double* errorFactorPtr)
{
	if (ApplyTransform(m_nodes, transformation, mode, errorFactorPtr)){
		istd::CChangeNotifier notifier(this, CF_OBJECT_POSITION | CF_MODEL);

		return true;
	}

	return false;
}


bool CPolygon::InvTransform(
			const ITransformation2d& transformation,
			ITransformation2d::ExactnessMode mode,
			double* errorFactorPtr)
{
	if (ApplyInverseTransform(m_nodes, transformation, mode, errorFactorPtr)){
		istd::CChangeNotifier notifier(this, CF_OBJECT_POSITION | CF_MODEL);

		return true;
	}

	return false;
}


bool CPolygon::GetTransformed(
			const ITransformation2d& transformation,
			IObject2d& result,
			ITransformation2d::ExactnessMode mode,
			double* errorFactorPtr) const
{
	CPolygon* polygonPtr = dynamic_cast<CPolygon*>(&result);
	if (polygonPtr != NULL){
		polygonPtr->m_nodes = m_nodes;

		if (ApplyTransform(polygonPtr->m_nodes, transformation, mode, errorFactorPtr)){
			istd::CChangeNotifier notifier(polygonPtr, CF_OBJECT_POSITION | CF_MODEL);

			return true;
		}
	}
		
	return false;
}


bool CPolygon::GetInvTransformed(
			const ITransformation2d& transformation,
			IObject2d& result,
			ITransformation2d::ExactnessMode mode,
			double* errorFactorPtr) const
{
	CPolygon* polygonPtr = dynamic_cast<CPolygon*>(&result);
	if (polygonPtr != NULL){
		polygonPtr->m_nodes = m_nodes;

		if (ApplyInverseTransform(polygonPtr->m_nodes, transformation, mode, errorFactorPtr)){
			istd::CChangeNotifier notifier(polygonPtr, CF_OBJECT_POSITION | CF_MODEL);

			return true;
		}
	}
		
	return false;
}


// reimplemented (istd::IChangeable)

int CPolygon::GetSupportedOperations() const
{
	return SO_COPY | SO_CLONE;
}


bool CPolygon::CopyFrom(const IChangeable& object, CompatibilityMode mode)
{
	const CPolygon* polygonPtr = dynamic_cast<const CPolygon*>(&object);

	if (polygonPtr != NULL){		
		istd::CChangeNotifier notifier(this);

		int sourceNodesCount = polygonPtr->GetNodesCount();
		for (int nodesIndex = 0; nodesIndex < sourceNodesCount; nodesIndex++){		
			InsertNode(polygonPtr->GetNode(nodesIndex));
		}

		CObject2dBase::CopyFrom(object, mode);

		return true;
	}	

	return false;
}


istd::IChangeable* CPolygon::CloneMe(CompatibilityMode mode) const 
{
	istd::TDelPtr<CPolygon> clonePtr(new CPolygon);

	if (clonePtr->CopyFrom(*this, mode)){
		return clonePtr.PopPtr();
	}

	return NULL;
}


// reimplemented (iser::ISerializable)

bool CPolygon::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag polygonTag("Polygon", "Polygon");
	static iser::CArchiveTag vectorTag("V", "Vector");

	istd::CChangeNotifier notifier(archive.IsStoring()? NULL: this, CF_OBJECT_POSITION | CF_MODEL);

	int nodesCount = m_nodes.size();
	bool retVal = true;

	retVal = retVal && archive.BeginMultiTag(polygonTag, vectorTag, nodesCount);
	if (!archive.IsStoring() && retVal){
		SetNodesCount(nodesCount);
	}

	for (int nodeIndex = 0; nodeIndex < nodesCount; ++nodeIndex){
		retVal = retVal && archive.BeginTag(vectorTag);
		retVal = retVal && m_nodes[nodeIndex].Serialize(archive);
		retVal = retVal && archive.EndTag(vectorTag);
	}
	retVal = retVal && archive.EndTag(polygonTag);

	return retVal;
}


// private static methods

bool CPolygon::ApplyTransform(Nodes& nodes,
			const ITransformation2d& transformation,
			ITransformation2d::ExactnessMode mode,
			double* errorFactorPtr)
{
	int nodesCount = nodes.count();
	
	QVector<i2d::CVector2d> transPoints;

	for (int nodeIndex = 0; nodeIndex < nodesCount; nodeIndex++){
		i2d::CVector2d transPoint;
		if (!transformation.GetPositionAt(nodes[nodeIndex], transPoint, mode)){
			return false;
		}

		transPoints.push_back(transPoint);
	}

	nodes = transPoints;

	if (errorFactorPtr != NULL){
		*errorFactorPtr = 0;
	}

	return true;
}


bool CPolygon::ApplyInverseTransform(
			Nodes& nodes,
			const ITransformation2d& transformation,
			ITransformation2d::ExactnessMode mode,
			double* errorFactorPtr)
{
	int nodesCount = nodes.count();
	
	QVector<i2d::CVector2d> transPoints;

	for (int nodeIndex = 0; nodeIndex < nodesCount; nodeIndex++){
		i2d::CVector2d transPoint;
		if (!transformation.GetInvPositionAt(nodes[nodeIndex], transPoint, mode)){
			return false;
		}

		transPoints.push_back(transPoint);
	}

	nodes = transPoints;

	if (errorFactorPtr != NULL){
		*errorFactorPtr = 0;
	}

	return true;
}


} // namespace i2d


