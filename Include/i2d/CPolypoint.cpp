#include <i2d/CPolypoint.h>


// Qt includes
#include <QtCore/QObject>

// ACF includes
#include <istd/TDelPtr.h>
#include <istd/CClassInfo.h>
#include <i2d/CLine2d.h>
#include <i2d/CAffineTransformation2d.h>


namespace i2d
{


// public static methods


QByteArray CPolypoint::GetTypeName()
{
	return istd::CClassInfo::GetName<CPolypoint>();
}


// public methods

void CPolypoint::Clear()
{
	if (!m_positions.empty()){
		istd::CChangeNotifier changeNotifier(this, &s_clearAllNodesChange);
		Q_UNUSED(changeNotifier);

		m_positions.clear();
	}
}


void CPolypoint::SetNodesCount(int nodesCount)
{
	if (nodesCount != (int)m_positions.size()){
		istd::CChangeNotifier changeNotifier(this, &s_createPolygonNodesChange);
		Q_UNUSED(changeNotifier);

		m_positions.resize(nodesCount);
	}
}


bool CPolypoint::InsertNode(const i2d::CVector2d& position)
{
	istd::CChangeNotifier changeNotifier(this, &s_insertPolygonNodeChange);
	Q_UNUSED(changeNotifier);

	m_positions.push_back(position);

	return true;
}


bool CPolypoint::InsertNode(int index, const i2d::CVector2d& position)
{
	istd::CChangeNotifier changeNotifier(this, &s_insertPolygonNodeChange);
	Q_UNUSED(changeNotifier);

	Nodes::iterator iter = m_positions.begin();
	iter += index;
	m_positions.insert(iter, position);

	return true;
}


bool CPolypoint::RemoveNode(int index)
{
	istd::CChangeNotifier changeNotifier(this, &s_removePolygonNodeChange);
	Q_UNUSED(changeNotifier);

	Nodes::iterator iter = m_positions.begin();
	iter += index;
	m_positions.erase(iter);

	return true;
}


// reimplemented (i2d::IObject2d)

CVector2d CPolypoint::GetCenter() const
{
	return GetBoundingBox().GetCenter();
}


void CPolypoint::MoveCenterTo(const CVector2d& position)
{
	i2d::CVector2d offset = position - GetCenter();
	if (offset != i2d::CVector2d(0, 0)){
		istd::CChangeNotifier changeNotifier(this, &s_objectMovedChange);
		Q_UNUSED(changeNotifier);

		int nodesCount = GetNodesCount();
		for (int i = 0; i < nodesCount; ++i){
			SetNodePos(i, GetNodePos(i) + offset);
		}
	}
}


i2d::CRectangle CPolypoint::GetBoundingBox() const
{
	int nodesCount = GetNodesCount();

	if (nodesCount > 0){
		const i2d::CVector2d& firstPoint = GetNodePos(0);

		i2d::CRectangle boundingBox(firstPoint, firstPoint);

		for (int i = 1; i < nodesCount; ++i){
			const i2d::CVector2d& point = GetNodePos(i);

			boundingBox.Unite(point);
		}

		return boundingBox;
	}

	return i2d::CRectangle::GetEmpty();
}


bool CPolypoint::Transform(
			const ITransformation2d& transformation,
			ITransformation2d::ExactnessMode mode,
			double* errorFactorPtr)
{
	istd::CChangeNotifier changeNotifier(this, &s_objectModifiedChange);

	if (ApplyTransform(m_positions, transformation, mode, errorFactorPtr)){
		return true;
	}

	changeNotifier.Abort();

	return false;
}


bool CPolypoint::InvTransform(
			const ITransformation2d& transformation,
			ITransformation2d::ExactnessMode mode,
			double* errorFactorPtr)
{
	istd::CChangeNotifier changeNotifier(this, &s_objectModifiedChange);

	if (ApplyInverseTransform(m_positions, transformation, mode, errorFactorPtr)){
		return true;
	}

	changeNotifier.Abort();

	return false;
}


bool CPolypoint::GetTransformed(
			const ITransformation2d& transformation,
			IObject2d& result,
			ITransformation2d::ExactnessMode mode,
			double* errorFactorPtr) const
{
	CPolypoint* polygonPtr = dynamic_cast<CPolypoint*>(&result);
	if (polygonPtr != NULL){
		Nodes nodes = m_positions;

		if (ApplyTransform(nodes, transformation, mode, errorFactorPtr)){
			istd::CChangeNotifier changeNotifier(polygonPtr, &s_objectModifiedChange);
			Q_UNUSED(changeNotifier);

			polygonPtr->m_positions = nodes;

			return true;
		}
	}

	return false;
}


bool CPolypoint::GetInvTransformed(
			const ITransformation2d& transformation,
			IObject2d& result,
			ITransformation2d::ExactnessMode mode,
			double* errorFactorPtr) const
{
	CPolypoint* polygonPtr = dynamic_cast<CPolypoint*>(&result);
	if (polygonPtr != NULL){
		Nodes nodes = m_positions;

		if (ApplyInverseTransform(nodes, transformation, mode, errorFactorPtr)){
			istd::CChangeNotifier changeNotifier(polygonPtr, &s_objectModifiedChange);
			Q_UNUSED(changeNotifier);

			polygonPtr->m_positions = nodes;

			return true;
		}
	}

	return false;
}


// reimplemented (iser::IObject)

QByteArray CPolypoint::GetFactoryId() const
{

	return GetTypeName();
}


// reimplemented (iser::ISerializable)

bool CPolypoint::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag polygonTag("Polygon", "Polygon", iser::CArchiveTag::TT_MULTIPLE);
	static iser::CArchiveTag pointsTag("Points", "List of node positions", iser::CArchiveTag::TT_MULTIPLE);
	static iser::CArchiveTag vectorTag("V", "Vector", iser::CArchiveTag::TT_GROUP, &polygonTag);

	quint32 versionNumber = 0;
	bool isOldFormat = archive.GetVersionInfo().GetVersionNumber(iser::IVersionInfo::AcfVersionId, versionNumber) && (versionNumber < 4194);

	istd::CChangeNotifier changeNotifier(archive.IsStoring()? NULL: this, &GetAllChanges());
	Q_UNUSED(changeNotifier);

	int nodesCount = int(m_positions.size());
	bool retVal = true;

	retVal = retVal && archive.BeginMultiTag(isOldFormat? polygonTag: pointsTag, vectorTag, nodesCount);
	if (!archive.IsStoring() && retVal){
		SetNodesCount(nodesCount);
	}

	for (int nodeIndex = 0; nodeIndex < nodesCount; ++nodeIndex){
		retVal = retVal && archive.BeginTag(vectorTag);
		retVal = retVal && m_positions[nodeIndex].Serialize(archive);
		retVal = retVal && archive.EndTag(vectorTag);
	}

	retVal = retVal && archive.EndTag(isOldFormat? polygonTag: pointsTag);

	return retVal;
}


// reimplemented (istd::IChangeable)

int CPolypoint::GetSupportedOperations() const
{
	return SO_COPY | SO_CLONE | SO_COMPARE;
}


bool CPolypoint::CopyFrom(const IChangeable& object, CompatibilityMode mode)
{
	const CPolypoint* polygonPtr = dynamic_cast<const CPolypoint*>(&object);

	if (polygonPtr != NULL){		
		istd::CChangeNotifier changeNotifier(this);
		Q_UNUSED(changeNotifier);

		m_positions = polygonPtr->m_positions;

		return CObject2dBase::CopyFrom(object, mode);
	}

	return false;
}


istd::IChangeable* CPolypoint::CloneMe(CompatibilityMode mode) const 
{
	istd::TDelPtr<CPolypoint> clonePtr(new CPolypoint);

	if (clonePtr->CopyFrom(*this, mode)){
		return clonePtr.PopPtr();
	}

	return NULL;
}

bool CPolypoint::IsEqual(const IChangeable& object) const
{
	const CPolypoint* polygonPtr = dynamic_cast<const CPolypoint*>(&object);
	if (polygonPtr != NULL){
		return (m_positions == polygonPtr->m_positions);
	}

	return false;
}


// private static methods

bool CPolypoint::ApplyTransform(Nodes& nodes,
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


bool CPolypoint::ApplyInverseTransform(
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


// static protected attributes

const istd::IChangeable::ChangeSet CPolypoint::s_clearAllNodesChange(CPolypoint::CF_OBJECT_POSITION, CPolypoint::CF_ALL_DATA, QObject::tr("Clear all nodes"));
const istd::IChangeable::ChangeSet CPolypoint::s_createPolygonNodesChange(CPolypoint::CF_OBJECT_POSITION, CPolypoint::CF_ALL_DATA, QObject::tr("Create nodes"));
const istd::IChangeable::ChangeSet CPolypoint::s_insertPolygonNodeChange(CPolypoint::CF_OBJECT_POSITION, CPolypoint::CF_ALL_DATA, QObject::tr("Insert node"));
const istd::IChangeable::ChangeSet CPolypoint::s_removePolygonNodeChange(CPolypoint::CF_OBJECT_POSITION, CPolypoint::CF_ALL_DATA, QObject::tr("Remove node"));


} // namespace i2d


