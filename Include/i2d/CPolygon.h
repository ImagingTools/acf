#ifndef i2d_CPolygon_included
#define i2d_CPolygon_included


// Qt includes
#include <QtCore/QVector>

// ACF includes
#include "iser/CArchiveTag.h"

#include "i2d/CVector2d.h"
#include "i2d/CRectangle.h"


namespace i2d
{


class CPolygon: virtual public IObject2d
{
public:
	CPolygon();

	/**
		Removes all nodes.
	*/
	virtual void Clear();

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

	/**
		Get outline length of this polygon.
	*/
	virtual double GetOutlineLength() const;

	/**
		Calculate bounding box (the same as above but returns result directly).
	*/
	virtual i2d::CRectangle GetBoundingBox() const;

	/**
		Assignment operator.
	*/
	const CPolygon& operator=(const CPolygon& polygon);

	// reimplemented (i2d::IObject2d)
	virtual CVector2d GetCenter() const;
	virtual void MoveCenterTo(const CVector2d& position);
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

protected:
	/**
		Set new nodes count.
	*/
	virtual void SetNodesCount(int nodesCount);

private:
	QVector<i2d::CVector2d> m_nodes;
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
	I_ASSERT(index >= 0 && index < int(m_nodes.size()));

	return m_nodes[index];
}


inline void CPolygon::SetNode(int index, const i2d::CVector2d& node)
{
	I_ASSERT(index >= 0 && index < int(m_nodes.size()));

	m_nodes[index] = node;
}


inline const CPolygon& CPolygon::operator=(const CPolygon& polygon)
{
	m_nodes = polygon.m_nodes;

	return *this;
}


} // namespace i2d


#endif // !i2d_CPolygon_included


