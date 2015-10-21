#include "i2d/CPolygon.h"


// Qt includes
#include <QtCore/QObject>

// ACF includes
#include "istd/TDelPtr.h"
#include "istd/CChangeNotifier.h"
#include "i2d/CLine2d.h"
#include "i2d/CAffineTransformation2d.h"


namespace i2d
{


static const istd::IChangeable::ChangeSet s_clearAllNodesChange(CPolygon::CF_OBJECT_POSITION, CPolygon::CF_ALL_DATA, QObject::tr("Clear all nodes"));
static const istd::IChangeable::ChangeSet s_createPolygonNodesChange(CPolygon::CF_OBJECT_POSITION, CPolygon::CF_ALL_DATA, QObject::tr("Create nodes"));
static const istd::IChangeable::ChangeSet s_insertPolygonNodeChange(CPolygon::CF_OBJECT_POSITION, CPolygon::CF_ALL_DATA, QObject::tr("Insert node"));
static const istd::IChangeable::ChangeSet s_removePolygonNodeChange(CPolygon::CF_OBJECT_POSITION, CPolygon::CF_ALL_DATA, QObject::tr("Remove node"));
static const istd::IChangeable::ChangeSet s_flipPolygonChange(CPolygon::CF_OBJECT_POSITION, CPolygon::CF_ALL_DATA, QObject::tr("Flip"));
static const istd::IChangeable::ChangeSet s_rotatePolygonChange(CPolygon::CF_OBJECT_POSITION, CPolygon::CF_ALL_DATA, QObject::tr("Rotate"));
static const istd::IChangeable::ChangeSet s_reversePolygonChange(CPolygon::CF_OBJECT_POSITION, CPolygon::CF_ALL_DATA, QObject::tr("Reverse nodes"));


// public methods

void CPolygon::Clear()
{
	if (!m_nodes.empty()){
		istd::CChangeNotifier changeNotifier(this, &s_clearAllNodesChange);
		Q_UNUSED(changeNotifier);

		m_nodes.clear();
	}
}


void CPolygon::SetNodesCount(int nodesCount)
{
	if (nodesCount != (int)m_nodes.size()){
		istd::CChangeNotifier changeNotifier(this, &s_createPolygonNodesChange);
		Q_UNUSED(changeNotifier);

		m_nodes.resize(nodesCount);
	}
}


bool CPolygon::InsertNode(const i2d::CVector2d& node)
{
	istd::CChangeNotifier changeNotifier(this, &s_insertPolygonNodeChange);
	Q_UNUSED(changeNotifier);

	m_nodes.push_back(node);

	return true;
}


bool CPolygon::InsertNode(int index, const i2d::CVector2d& node)
{
	istd::CChangeNotifier changeNotifier(this, &s_insertPolygonNodeChange);
	Q_UNUSED(changeNotifier);

	Nodes::iterator iter = m_nodes.begin();
	iter += index;
	m_nodes.insert(iter, node);

	return true;
}


bool CPolygon::RemoveNode(int index)
{
	istd::CChangeNotifier changeNotifier(this, &s_removePolygonNodeChange);
	Q_UNUSED(changeNotifier);

	Nodes::iterator iter = m_nodes.begin();
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


void CPolygon::FlipByX()
{
	int count = GetNodesCount();
	if (count){
		istd::CChangeNotifier changeNotifier(this, &s_flipPolygonChange);
		Q_UNUSED(changeNotifier);

		i2d::CVector2d center = GetCenter();

		for (int i = 0; i < count; i++){
			m_nodes[i].SetX(center.GetX() + (center.GetX() - m_nodes[i].GetX()));
		}
	}
}


void CPolygon::FlipByY()
{
	int count = GetNodesCount();
	if (count){
		istd::CChangeNotifier changeNotifier(this, &s_flipPolygonChange);
		Q_UNUSED(changeNotifier);

		i2d::CVector2d center = GetCenter();

		for (int i = 0; i < count; i++){
			m_nodes[i].SetY(center.GetY() + (center.GetY() - m_nodes[i].GetY()));
		}
	}
}


void CPolygon::Rotate(double radians)
{
	int count = GetNodesCount();
	if (count){
		istd::CChangeNotifier changeNotifier(this, &s_rotatePolygonChange);
		Q_UNUSED(changeNotifier);

		i2d::CVector2d center = GetCenter();

		i2d::CAffineTransformation2d translateTo00;
		translateTo00.Reset(-center);
		i2d::CAffineTransformation2d rotate;
		rotate.Reset(i2d::CVector2d(0, 0), radians);
		i2d::CAffineTransformation2d translateBackToCenter;
		translateBackToCenter.Reset(center);
		Transform(translateTo00);
		Transform(rotate);
		Transform(translateBackToCenter);
	}
}


void CPolygon::ReverseNodes()
{
	int count = GetNodesCount();
	if (count){
		istd::CChangeNotifier changeNotifier(this, &s_reversePolygonChange);
		Q_UNUSED(changeNotifier);

		for (int i = 0; i < count / 2; i++){
			i2d::CVector2d node1 = GetNode(i);
			i2d::CVector2d node2 = GetNode(count - 1 - i);
			SetNode(i, node2);
			SetNode(count - 1 - i, node1);
		}
	}
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
		istd::CChangeNotifier changeNotifier(this, &s_objectMovedChange);
		Q_UNUSED(changeNotifier);

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
	istd::CChangeNotifier changeNotifier(this, &s_objectModifiedChange);

	if (ApplyTransform(m_nodes, transformation, mode, errorFactorPtr)){
		return true;
	}

	changeNotifier.Abort();

	return false;
}


bool CPolygon::InvTransform(
			const ITransformation2d& transformation,
			ITransformation2d::ExactnessMode mode,
			double* errorFactorPtr)
{
	istd::CChangeNotifier changeNotifier(this, &s_objectModifiedChange);

	if (ApplyInverseTransform(m_nodes, transformation, mode, errorFactorPtr)){
		return true;
	}

	changeNotifier.Abort();

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
		Nodes nodes = m_nodes;

		if (ApplyTransform(nodes, transformation, mode, errorFactorPtr)){
			istd::CChangeNotifier changeNotifier(polygonPtr, &s_objectModifiedChange);
			Q_UNUSED(changeNotifier);

			polygonPtr->m_nodes = nodes;

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
		Nodes nodes = m_nodes;

		if (ApplyInverseTransform(nodes, transformation, mode, errorFactorPtr)){
			istd::CChangeNotifier changeNotifier(polygonPtr, &s_objectModifiedChange);
			Q_UNUSED(changeNotifier);

			polygonPtr->m_nodes = nodes;

			return true;
		}
	}

	return false;
}


// reimplemented (iser::ISerializable)

bool CPolygon::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag polygonTag("Polygon", "Polygon", iser::CArchiveTag::TT_MULTIPLE);
	static iser::CArchiveTag vectorTag("V", "Vector", iser::CArchiveTag::TT_GROUP, &polygonTag);

	istd::CChangeNotifier changeNotifier(archive.IsStoring()? NULL: this, &GetAllChanges());
	Q_UNUSED(changeNotifier);

	int nodesCount = int(m_nodes.size());
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


// reimplemented (istd::IChangeable)

int CPolygon::GetSupportedOperations() const
{
	return SO_COPY | SO_CLONE | SO_COMPARE;
}


bool CPolygon::CopyFrom(const IChangeable& object, CompatibilityMode mode)
{
	const CPolygon* polygonPtr = dynamic_cast<const CPolygon*>(&object);

	if (polygonPtr != NULL){		
		istd::CChangeNotifier changeNotifier(this);
		Q_UNUSED(changeNotifier);

		m_nodes = polygonPtr->m_nodes;

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

bool CPolygon::IsEqual(const IChangeable& object) const
{
	const CPolygon* polygonPtr = dynamic_cast<const CPolygon*>(&object);
	if (polygonPtr != NULL)
	{
		return (m_nodes == polygonPtr->m_nodes);
	}

	return false;
}


// private static methods

bool CPolygon::ApplyTransform(Nodes& nodes,
			const ITransformation2d& transformation,
			ITransformation2d::ExactnessMode mode,
			double* errorFactorPtr)
{
	int nodesCount = int(nodes.size());
	
    Nodes transPoints;

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
	int nodesCount = int(nodes.size());
	
    Nodes transPoints;

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


