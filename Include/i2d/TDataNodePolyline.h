#ifndef i2d_TDataNodePolyline_included
#define i2d_TDataNodePolyline_included


// ACF includes
#include "i2d/CDataNodePolylineBase.h"


namespace i2d
{


/**
	Generic polyline implementation with additional information stored for each node.
	\note Template parameter \c NodeData must be derrived from iser::ISerializable.
*/
template<class NodeData>
class TDataNodePolyline: public CDataNodePolylineBase
{
public:
	typedef CPolyline BaseClass;

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
	I_ASSERT(nodeIndex >= 0);
	I_ASSERT(nodeIndex < int(m_nodesData.size()));

	return m_nodesData[nodeIndex];
}


template<class NodeData>
inline NodeData& TDataNodePolyline<NodeData>::GetTNodeDataRef(int nodeIndex)
{
	I_ASSERT(nodeIndex >= 0);
	I_ASSERT(nodeIndex < int(m_nodesData.size()));

	return m_nodesData[nodeIndex];
}


// reimplemented (i2d::CDataNodePolylineBase)

template<class NodeData>
inline const iser::ISerializable& TDataNodePolyline<NodeData>::GetNodeData(int nodeIndex) const
{
	I_ASSERT(nodeIndex >= 0);
	I_ASSERT(nodeIndex < int(m_nodesData.size()));

	return m_nodesData[nodeIndex];
}


template<class NodeData>
inline iser::ISerializable& TDataNodePolyline<NodeData>::GetNodeDataRef(int nodeIndex)
{
	I_ASSERT(nodeIndex >= 0);
	I_ASSERT(nodeIndex < int(m_nodesData.size()));

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
	NodesData::iterator iter = m_nodesData.begin();
	iter += index;
	m_nodesData.insert(iter, NodeData());

	return BaseClass::InsertNode(index, node);
}


template<class NodeData>
bool TDataNodePolyline<NodeData>::RemoveNode(int index)
{
	NodesData::iterator iter = m_nodesData.begin();
	iter += index;
	m_nodesData.erase(iter);

	return BaseClass::RemoveNode(index);
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


