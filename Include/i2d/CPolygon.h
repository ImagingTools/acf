#ifndef i2d_CPolygon_included
#define i2d_CPolygon_included


// Qt includes
#include <QtCore/QVector>

// ACF includes
#include "iser/CArchiveTag.h"
#include "i2d/CObject2dBase.h"
#include "i2d/CVector2d.h"
#include "i2d/CRectangle.h"


namespace i2d
{


/**
	Definition of the data model for a polygon.
*/
class CPolygon: public CObject2dBase
{
public:
	CPolygon();

	/**
		Removes all nodes.
	*/
	virtual void Clear();

	/**
		Set new nodes count.
	*/
	virtual void SetNodesCount(int nodesCount);

	/**
		Return size of node table.
	*/
	virtual int GetNodesCount() const;

	/**
		Return node at specified index.
		\param	index	an index in node table.
	 */
	virtual const i2d::CVector2d& GetNode(int index) const;

	/**
		Set node at specified index.
		\param	index	Index in node table.
		\param	node	New node value.
	*/
	virtual void SetNode(int index, const i2d::CVector2d& node);

	/**
		Insert a node at the end of node table.
	*/
	virtual bool InsertNode(const i2d::CVector2d& node);

	/**
		Insert a node at specified index.
	*/
	virtual bool InsertNode(int index, const i2d::CVector2d& node);

	/**
		Remove a node at specified index.
	*/
	virtual bool RemoveNode(int index);

	// Following "quiet" methods are intended to directly change data without change notification (for performance reasons)
	void InsertNodeQuiet(const i2d::CVector2d& node);
	void InsertNodeQuiet(int index, const i2d::CVector2d& node);
	void SetNodesCountQuiet(int count);

	/**
		Get outline length of this polygon.
	*/
	virtual double GetOutlineLength() const;

	// reimplemented (i2d::IObject2d)
	virtual CVector2d GetCenter() const;
	virtual void MoveCenterTo(const CVector2d& position);
	/**
		Calculate bounding box (the same as above but returns result directly).
	*/
	virtual i2d::CRectangle GetBoundingBox() const;
	virtual bool Transform(
				const ITransformation2d& transformation,
				ITransformation2d::ExactnessMode mode = ITransformation2d::EM_NONE,
				double* errorFactorPtr = NULL);
	virtual bool InvTransform(
				const ITransformation2d& transformation,
				ITransformation2d::ExactnessMode mode = ITransformation2d::EM_NONE,
				double* errorFactorPtr = NULL);
	virtual bool GetTransformed(
				const ITransformation2d& transformation,
				IObject2d& result,
				ITransformation2d::ExactnessMode mode = ITransformation2d::EM_NONE,
				double* errorFactorPtr = NULL) const;
	virtual bool GetInvTransformed(
				const ITransformation2d& transformation,
				IObject2d& result,
				ITransformation2d::ExactnessMode mode = ITransformation2d::EM_NONE,
				double* errorFactorPtr = NULL) const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

	//reimplemented istd::IChangeable
	virtual int GetSupportedOperations() const;
	virtual bool CopyFrom(const IChangeable& object, CompatibilityMode mode = CM_WITHOUT_REFS);
	virtual istd::IChangeable* CloneMe(CompatibilityMode mode = CM_WITHOUT_REFS) const;
	virtual bool IsEqual(const IChangeable& object) const;

private:
	// std::vector can be faster than QVector
	typedef std::vector<i2d::CVector2d> Nodes;

	/**
		Apply 2D-transformation to the list of nodes.
	*/
	static bool ApplyTransform(Nodes& nodes,
				const ITransformation2d& transformation,
				ITransformation2d::ExactnessMode mode = ITransformation2d::EM_NONE,
				double* errorFactorPtr = NULL);

	/**
		Apply inverse 2D-transformation to the list of nodes.
	*/
	static bool ApplyInverseTransform(
				Nodes& nodes,
				const ITransformation2d& transformation,
				ITransformation2d::ExactnessMode mode = ITransformation2d::EM_NONE,
				double* errorFactorPtr = NULL);

private:
	Nodes m_nodes;
};


// constructors

inline CPolygon::CPolygon()
{
}


// inline functions

inline int CPolygon::GetNodesCount() const
{
	return int(m_nodes.size());
}


inline const i2d::CVector2d& CPolygon::GetNode(int index) const
{
	Q_ASSERT(index >= 0 && index < int(m_nodes.size()));

	return m_nodes[index];
}


inline void CPolygon::SetNode(int index, const i2d::CVector2d& node)
{
	Q_ASSERT(index >= 0 && index < int(m_nodes.size()));

	m_nodes[index] = node;
}


inline void CPolygon::InsertNodeQuiet(const i2d::CVector2d& node)
{
	m_nodes.push_back(node);
}


inline void CPolygon::InsertNodeQuiet(int index, const i2d::CVector2d& node)
{
	Nodes::iterator iter = m_nodes.begin();
	iter += index;
	m_nodes.insert(iter, node);
}


inline void CPolygon::SetNodesCountQuiet(int count)
{
	m_nodes.resize(count);
}



} // namespace i2d


#endif // !i2d_CPolygon_included


