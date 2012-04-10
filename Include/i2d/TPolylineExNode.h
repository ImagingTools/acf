#ifndef i2d_TPolylineExNode_included
#define i2d_TPolylineExNode_included


// ACF includes
#include "i2d/CPolylineExNodeBase.h"


namespace i2d
{


template<class NodeData>
class TPolylineExNode: public CPolylineExNodeBase
{
public:
	typedef CPolyline BaseClass;

    /** Get segment user data.
     */
    const NodeData& GetTNodeData(int nodeIndex) const;
    /** Get reference of segment user data.
     */
    NodeData& GetTNodeDataRef(int nodeIndex);

    // reimplement (i2d::CPolylineExNodeBase)
    virtual const iser::ISerializable& GetNodeData(int nodeIndex) const;
    virtual iser::ISerializable& GetNodeDataRef(int nodeIndex);

    // reimplement (i2d::CPolygon)
	virtual bool InsertNode(const i2d::CVector2d& node);
	virtual bool InsertNode(int index, const i2d::CVector2d& node);
	virtual void Clear();
	virtual bool RemoveNode(int index);
	
protected:
    // reimplement (i2d::CPolygon)
	virtual void SetNodesCount(int count);

private:
	typedef ::QVector<NodeData> NodesData;
	NodesData m_nodesData;
};


template<class NodeData>
inline const NodeData& TPolylineExNode<NodeData>::GetTNodeData(int nodeIndex) const
{
	I_ASSERT(nodeIndex >= 0);
	I_ASSERT(nodeIndex < int(m_nodesData.size()));

	return m_nodesData[nodeIndex];
}


template<class NodeData>
inline NodeData& TPolylineExNode<NodeData>::GetTNodeDataRef(int nodeIndex)
{
	I_ASSERT(nodeIndex >= 0);
	I_ASSERT(nodeIndex < int(m_nodesData.size()));

	return m_nodesData[nodeIndex];
}


// reimplement (i2d::CPolylineExNodeBase)

template<class NodeData>
inline const iser::ISerializable& TPolylineExNode<NodeData>::GetNodeData(int nodeIndex) const
{
	I_ASSERT(nodeIndex >= 0);
	I_ASSERT(nodeIndex < int(m_nodesData.size()));

	return m_nodesData[nodeIndex];
}


template<class NodeData>
inline iser::ISerializable& TPolylineExNode<NodeData>::GetNodeDataRef(int nodeIndex)
{
	I_ASSERT(nodeIndex >= 0);
	I_ASSERT(nodeIndex < int(m_nodesData.size()));

	return m_nodesData[nodeIndex];
}


// reimplement (i2d::CPolygon)

template<class NodeData>
inline bool TPolylineExNode<NodeData>::InsertNode(const i2d::CVector2d& node)
{
	m_nodesData.insert(m_nodesData.end(), NodeData());

	return BaseClass::InsertNode(node);
}


template<class NodeData>
inline bool TPolylineExNode<NodeData>::InsertNode(int index, const i2d::CVector2d& node)
{
	NodesData::iterator iter = m_nodesData.begin();
	iter += index;
	m_nodesData.insert(iter, NodeData());

	return BaseClass::InsertNode(index, node);
}


template<class NodeData>
inline void TPolylineExNode<NodeData>::Clear()
{
	m_nodesData.clear();
	BaseClass::Clear();
}


template<class NodeData>
bool TPolylineExNode<NodeData>::RemoveNode(int index)
{
	NodesData::iterator iter = m_nodesData.begin();
	iter += index;
	m_nodesData.erase(iter);

	return BaseClass::RemoveNode(index);
}


// protected methods

// reimplement (i2d::CPolygon)

template<class NodeData>
void TPolylineExNode<NodeData>::SetNodesCount(int count)
{
	m_nodesData.resize(count);

	BaseClass::SetNodesCount(count);
}


} // namespace i2d


#endif // !i2d_TPolylineExNode_included



