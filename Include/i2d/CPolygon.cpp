#include "i2d/CPolygon.h"


// ACF includes
#include "i2d/CLine2d.h"

#include "istd/TChangeNotifier.h"


namespace i2d
{


// public methods

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


void CPolygon::Clear()
{
	m_nodes.clear();
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


i2d::CRectangle CPolygon::GetBoundingBox() const
{
	int nodesCount = GetNodesCount();

	if (nodesCount > 0){
		i2d::CVector2d sp = GetNode(0);
		i2d::CRectangle boundingBox(sp, sp);
		for (int i = 1; i < nodesCount; i++){
			sp = GetNode(i);
			boundingBox.Unite(sp);
		}
		return boundingBox;
	}

	return i2d::CRectangle();
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
		istd::CChangeNotifier notifier(this, CF_OBJECT_POSITION | istd::IChangeable::CF_MODEL);

		int nodesCount = GetNodesCount();
		for (int i = 0; i < nodesCount; i++){
			SetNode(i, GetNode(i) + offset);
		}
	}
}


bool CPolygon::Transform(
						   const ITransformation2d& /*transformation*/,
						   ITransformation2d::ExactnessMode /*mode*/,
						   double* /*errorFactorPtr*/)
{
	// TODO: implement geometrical transformations for rectangle.
	return false;
}


bool CPolygon::InvTransform(
							  const ITransformation2d& /*transformation*/,
							  ITransformation2d::ExactnessMode /*mode*/,
							  double* /*errorFactorPtr*/)
{
	// TODO: implement geometrical transformations for rectangle.
	return false;
}


bool CPolygon::GetTransformed(
								const ITransformation2d& /*transformation*/,
								IObject2d& /*result*/,
								ITransformation2d::ExactnessMode /*mode*/,
								double* /*errorFactorPtr*/) const
{
	// TODO: implement geometrical transformations for rectangle.
	return false;
}


bool CPolygon::GetInvTransformed(
								   const ITransformation2d& /*transformation*/,
								   IObject2d& /*result*/,
								   ITransformation2d::ExactnessMode /*mode*/,
								   double* /*errorFactorPtr*/) const
{
	// TODO: implement geometrical transformations for rectangle.
	return false;
}


// reimplemented (iser::ISerializable)

bool CPolygon::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag polygonTag("Polygon", "Polygon");
	static iser::CArchiveTag vectorTag("V", "Vector");

	istd::CChangeNotifier notifier(archive.IsStoring()? NULL: this, CF_OBJECT_POSITION | istd::IChangeable::CF_MODEL);

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


// protected methods

void CPolygon::SetNodesCount(int nodesCount)
{
	m_nodes.resize(nodesCount);
}


} // namespace i2d

