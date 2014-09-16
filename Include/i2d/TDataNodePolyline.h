#ifndef i2d_TDataNodePolyline_included
#define i2d_TDataNodePolyline_included


// ACF includes
#include "i2d/CDataNodePolylineBase.h"

#include "istd/CChangeNotifier.h"


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

	// reimplemented (i2d::CPolygon)
	virtual void Clear();
	virtual bool InsertNode(const i2d::CVector2d& node);
	virtual bool InsertNode(int index, const i2d::CVector2d& node);
	virtual bool RemoveNode(int index);

	// reimplemented istd::IChangeable
	virtual bool CopyFrom(const IChangeable& object, CompatibilityMode mode = CM_WITHOUT_REFS);

protected:
	// reimplemented (i2d::CPolygon)
	virtual void SetNodesCount(int count);

private:
	typedef QVector<NodeData> NodesData;
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


// reimplemented (i2d::CPolygon)

template<class NodeData>
inline void TDataNodePolyline<NodeData>::Clear()
{
	m_nodesData.clear();

	BaseClass::Clear();
}


template<class NodeData>
inline bool TDataNodePolyline<NodeData>::InsertNode(const i2d::CVector2d& node)
{
	m_nodesData.insert(m_nodesData.end(), NodeData());

	return BaseClass::InsertNode(node);
}


template<class NodeData>
inline bool TDataNodePolyline<NodeData>::InsertNode(int index, const i2d::CVector2d& node)
{
	typename NodesData::iterator iter = m_nodesData.begin();
	iter += index;
	m_nodesData.insert(iter, NodeData());

	return BaseClass::InsertNode(index, node);
}


template<class NodeData>
bool TDataNodePolyline<NodeData>::RemoveNode(int index)
{
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
		BeginChanges(GetAnyChange());

		int sourceNodesCount = polygonPtr->GetNodesCount();
		for (int nodesIndex = 0; nodesIndex < sourceNodesCount; nodesIndex++){		
			InsertNode(polygonPtr->GetNode(nodesIndex));

			m_nodesData[nodesIndex] = polygonPtr->m_nodesData[nodesIndex];
		}

		CObject2dBase::CopyFrom(object, mode);

		EndChanges(GetAnyChange());

		return true;
	}

	return false;
}


// protected methods

// reimplemented (i2d::CPolygon)

template<class NodeData>
void TDataNodePolyline<NodeData>::SetNodesCount(int count)
{
	m_nodesData.resize(count);

	BaseClass::SetNodesCount(count);
}


} // namespace i2d


#endif // !i2d_TDataNodePolyline_included


