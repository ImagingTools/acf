#ifndef i2d_TDataNodePolyline_included
#define i2d_TDataNodePolyline_included


// STL includes
#include <vector>

// ACF includes
#include <i2d/CDataNodePolylineBase.h>
#include <istd/CChangeNotifier.h>


namespace i2d
{


/**
	Generic polyline implementation with additional information stored for each node.
	\note Template parameter \c NodeData must be derived from iser::ISerializable.
*/
template<class NodeData>
class TDataNodePolyline: public CDataNodePolylineBase
{
public:
	typedef CDataNodePolylineBase BaseClass;

	/**
		Get user data from the given node.
	*/
	const NodeData& GetTNodeData(int nodeIndex) const;

	/**
		Get reference of user data in the given node.
	*/
	NodeData& GetTNodeDataRef(int nodeIndex);

	// reimplemented (i2d::CDataNodePolylineBase)
	virtual const iser::ISerializable& GetNodeData(int nodeIndex) const;
	virtual iser::ISerializable& GetNodeDataRef(int nodeIndex);

	// reimplemented (i2d::CPolypoint)
	virtual void Clear();
	virtual void SetNodesCount(int nodesCount);
	virtual bool InsertNode(const i2d::CVector2d& position);
	virtual bool InsertNode(int index, const i2d::CVector2d& position);
	virtual bool RemoveNode(int index);

	// reimplemented istd::IChangeable
	virtual bool CopyFrom(const IChangeable& object, CompatibilityMode mode = CM_WITHOUT_REFS);

private:
	typedef std::vector<NodeData> NodesData;
	NodesData m_nodesData;
};


template<class NodeData>
inline const NodeData& TDataNodePolyline<NodeData>::GetTNodeData(int nodeIndex) const
{
	Q_ASSERT(nodeIndex >= 0);
	Q_ASSERT(nodeIndex < int(m_nodesData.size()));

	return m_nodesData[nodeIndex];
}


template<class NodeData>
inline NodeData& TDataNodePolyline<NodeData>::GetTNodeDataRef(int nodeIndex)
{
	Q_ASSERT(nodeIndex >= 0);
	Q_ASSERT(nodeIndex < int(m_nodesData.size()));

	return m_nodesData[nodeIndex];
}


// reimplemented (i2d::CDataNodePolylineBase)

template<class NodeData>
inline const iser::ISerializable& TDataNodePolyline<NodeData>::GetNodeData(int nodeIndex) const
{
	Q_ASSERT(nodeIndex >= 0);
	Q_ASSERT(nodeIndex < int(m_nodesData.size()));

	return m_nodesData[nodeIndex];
}


template<class NodeData>
inline iser::ISerializable& TDataNodePolyline<NodeData>::GetNodeDataRef(int nodeIndex)
{
	Q_ASSERT(nodeIndex >= 0);
	Q_ASSERT(nodeIndex < int(m_nodesData.size()));

	return m_nodesData[nodeIndex];
}


// reimplemented (i2d::CPolypoint)

template<class NodeData>
inline void TDataNodePolyline<NodeData>::Clear()
{
	if (!m_nodesData.empty()){
		istd::CChangeNotifier changeNotifier(this, &s_clearAllNodesChange);
		Q_UNUSED(changeNotifier);

		m_nodesData.clear();

		BaseClass::Clear();
	}
}


template<class NodeData>
void TDataNodePolyline<NodeData>::SetNodesCount(int nodesCount)
{
	istd::CChangeNotifier changeNotifier(this, &s_createPolygonNodesChange);
	Q_UNUSED(changeNotifier);

	m_nodesData.resize(nodesCount);

	BaseClass::SetNodesCount(nodesCount);
}


template<class NodeData>
inline bool TDataNodePolyline<NodeData>::InsertNode(const i2d::CVector2d& position)
{
	istd::CChangeNotifier changeNotifier(this, &s_insertPolygonNodeChange);
	Q_UNUSED(changeNotifier);

	m_nodesData.insert(m_nodesData.end(), NodeData());

	return BaseClass::InsertNode(position);
}


template<class NodeData>
inline bool TDataNodePolyline<NodeData>::InsertNode(int index, const i2d::CVector2d& position)
{
	istd::CChangeNotifier changeNotifier(this, &s_insertPolygonNodeChange);
	Q_UNUSED(changeNotifier);

	typename NodesData::iterator iter = m_nodesData.begin();
	iter += index;
	m_nodesData.insert(iter, NodeData());

	return BaseClass::InsertNode(index, position);
}


template<class NodeData>
bool TDataNodePolyline<NodeData>::RemoveNode(int index)
{
	istd::CChangeNotifier changeNotifier(this, &s_removePolygonNodeChange);
	Q_UNUSED(changeNotifier);

	typename NodesData::iterator iter = m_nodesData.begin();
	iter += index;
	m_nodesData.erase(iter);

	return BaseClass::RemoveNode(index);
}


// reimplemented istd::IChangeable

template<class NodeData>
bool TDataNodePolyline<NodeData>::CopyFrom(const IChangeable& object, CompatibilityMode mode)
{
	Clear();

	const TDataNodePolyline<NodeData>* polygonPtr = dynamic_cast<const TDataNodePolyline<NodeData>*>(&object);

	if (polygonPtr != NULL){
		istd::CChangeNotifier changeNotifier(this);

		int sourceNodesCount = polygonPtr->GetNodesCount();
		for (int nodesIndex = 0; nodesIndex < sourceNodesCount; nodesIndex++){		
			InsertNode(polygonPtr->GetNodePos(nodesIndex));

			m_nodesData[nodesIndex] = polygonPtr->m_nodesData[nodesIndex];
		}

		CObject2dBase::CopyFrom(object, mode);

		return true;
	}

	return false;
}


} // namespace i2d


#endif // !i2d_TDataNodePolyline_included


