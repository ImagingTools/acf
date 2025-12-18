#ifndef i2d_CPolypoint_included
#define i2d_CPolypoint_included


// ACF includes
#include <istd/CChangeNotifier.h>
#include <iser/CArchiveTag.h>
#include <i2d/CObject2dBase.h>
#include <i2d/CVector2d.h>
#include <i2d/CRectangle.h>


namespace i2d
{


/**
	Definition of the data model for a polygon.
*/
class CPolypoint: public CObject2dBase
{
public:
	static QByteArray GetTypeName();

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
		Return \c true if there are no points.
	*/
	bool IsEmpty() const;

	/**
		Return position of node at specified index.
		\param	index	an index in node table.
	*/
	virtual const i2d::CVector2d& GetNodePos(int index) const;

	/**
		Get reference to position object for node.
		Please note, that the change notification cannot be done if you use this method to change node position.
		\param	index	an index in node table.
	*/
	i2d::CVector2d& GetNodePosRef(int index);

	/**
		Set node at specified index.
		\param	index	Index in node table.
		\param	node	New node value.
	*/
	virtual void SetNodePos(int index, const i2d::CVector2d& position);

	/**
		Insert a node at the end of node table.
	*/
	virtual bool InsertNode(const i2d::CVector2d& position);

	/**
		Insert a node at specified index.
	*/
	virtual bool InsertNode(int index, const i2d::CVector2d& position);

	/**
		Remove a node at specified index.
	*/
	virtual bool RemoveNode(int index);

	// reimplemented (i2d::IObject2d)
	virtual CVector2d GetCenter() const override;
	virtual void MoveCenterTo(const CVector2d& position) override;
	virtual i2d::CRectangle GetBoundingBox() const override;
	virtual bool Transform(
				const ITransformation2d& transformation,
				ITransformation2d::ExactnessMode mode = ITransformation2d::EM_NONE,
				double* errorFactorPtr = NULL) override;
	virtual bool InvTransform(
				const ITransformation2d& transformation,
				ITransformation2d::ExactnessMode mode = ITransformation2d::EM_NONE,
				double* errorFactorPtr = NULL) override;
	virtual bool GetTransformed(
				const ITransformation2d& transformation,
				IObject2d& result,
				ITransformation2d::ExactnessMode mode = ITransformation2d::EM_NONE,
				double* errorFactorPtr = NULL) const override;
	virtual bool GetInvTransformed(
				const ITransformation2d& transformation,
				IObject2d& result,
				ITransformation2d::ExactnessMode mode = ITransformation2d::EM_NONE,
				double* errorFactorPtr = NULL) const override;

	// reimplemented (iser::IObject)
	virtual QByteArray GetFactoryId() const override;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive) override;

	// reimplemented (istd::IChangeable)
	virtual int GetSupportedOperations() const override;
	virtual bool CopyFrom(const IChangeable& object, CompatibilityMode mode = CM_WITHOUT_REFS) override;
	virtual istd::TUniqueInterfacePtr<istd::IChangeable> CloneMe(CompatibilityMode mode = CM_WITHOUT_REFS) const override;
	virtual bool IsEqual(const IChangeable& object) const override;

protected:
	static const istd::IChangeable::ChangeSet s_clearAllNodesChange;
	static const istd::IChangeable::ChangeSet s_createPolygonNodesChange;
	static const istd::IChangeable::ChangeSet s_insertPolygonNodeChange;
	static const istd::IChangeable::ChangeSet s_removePolygonNodeChange;

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

	Nodes m_positions;
};


// inline functions

inline int CPolypoint::GetNodesCount() const
{
	return int(m_positions.size());
}


inline bool CPolypoint::IsEmpty() const
{
	return m_positions.empty();
}


inline const i2d::CVector2d& CPolypoint::GetNodePos(int index) const
{
	Q_ASSERT(index >= 0 && index < int(m_positions.size()));

	return m_positions[index];
}


inline i2d::CVector2d& CPolypoint::GetNodePosRef(int index)
{
	Q_ASSERT(index >= 0 && index < int(m_positions.size()));

	return m_positions[index];
}


inline void CPolypoint::SetNodePos(int index, const i2d::CVector2d& position)
{
	Q_ASSERT(index >= 0 && index < int(m_positions.size()));

	istd::CChangeNotifier changeNotifier(this);
	Q_UNUSED(changeNotifier);

	m_positions[index] = position;
}


} // namespace i2d


#endif // !i2d_CPolypoint_included


