#include <i2d/CTubePolylineComp.h>


namespace i2d
{


// public methods

// reimplemented (i2d::CPolygon)

bool CTubePolylineComp::InsertNode(const i2d::CVector2d& node)
{
	if (BaseClass2::InsertNode(node)){
		if (!m_defaultTubeRange.IsEmpty() && m_defaultTubeRange.IsValid()){
			CTubeNode& nodeDataRef = GetTNodeDataRef(GetNodesCount() - 1);

			nodeDataRef.SetTubeRange(m_defaultTubeRange);
		}

		return true;
	}

	return false;
}


bool CTubePolylineComp::InsertNode(int index, const i2d::CVector2d& node)
{
	if (BaseClass2::InsertNode(index, node)){
		if (!m_defaultTubeRange.IsEmpty() && m_defaultTubeRange.IsValid()){
			CTubeNode& nodeDataRef = GetTNodeDataRef(index);

			nodeDataRef.SetTubeRange(m_defaultTubeRange);
		}

		return true;
	}

	return false;
}


// protected methods

// reimplemented (i2d::CPolygon)

void CTubePolylineComp::SetNodesCount(int count)
{
	BaseClass2::SetNodesCount(count);

	if (!m_defaultTubeRange.IsEmpty() && m_defaultTubeRange.IsValid()){
		for (int nodeIndex = 0; nodeIndex < count; nodeIndex++){
			CTubeNode& node = GetTNodeDataRef(nodeIndex);

			node.SetTubeRange(m_defaultTubeRange);
		}
	}
}


// reimplemented (icomp::CComponentBase)

void CTubePolylineComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_minTubeRangeValueAttrPtr.IsValid() && m_maxTubeRangeValueAttrPtr.IsValid()){
		m_defaultTubeRange = istd::CRange(
					qMin(*m_minTubeRangeValueAttrPtr, *m_maxTubeRangeValueAttrPtr),
					qMax(*m_minTubeRangeValueAttrPtr, *m_maxTubeRangeValueAttrPtr));
	}

	if (		GetNodesCount() == 0 && 
				m_defaultAXPtr.IsValid() && m_defaultAYPtr.IsValid() && 
				m_defaultBXPtr.IsValid() && m_defaultBYPtr.IsValid()){
		InsertNode(CVector2d(*m_defaultAXPtr, *m_defaultAYPtr));
		InsertNode(CVector2d(*m_defaultBXPtr, *m_defaultBYPtr));
	}
}


} // namespace i2d


