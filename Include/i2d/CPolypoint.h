#ifndef i2d_CPolypoint_included
#define i2d_CPolypoint_included


// Qt includes
#include <QtCore/QVector>

// ACF includes
#include "iser/CArchiveTag.h"
#include "i2d/CObject2dBase.h"
#include "i2d/CVector2d.h"
#include "istd/TDelPtr.h"


namespace i2d
{


/**
	2D-object given by the simple set of points.
*/
class CPolypoint: public CObject2dBase
{
public:
	CPolypoint();

	const i2d::CVector2d& GetPoint(int index) const;
	int GetPointsCount() const;
	void Insert(const i2d::CVector2d& vector);
	void Clear();
	const QVector<i2d::CVector2d>& GetPoints() const;

	// reimplemented (i2d::IObject2d)
	virtual CVector2d GetCenter() const;
	virtual void MoveCenterTo(const CVector2d& position);
	virtual CRectangle GetBoundingBox() const;
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

	//reimplemented istd::IChangeable
	virtual int GetSupportedOperations() const;
	virtual bool CopyFrom(const IChangeable& object);
	virtual istd::IChangeable* CloneMe() const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	typedef QVector<i2d::CVector2d> Points;

	Points m_points;
};


// inline methods

inline CPolypoint::CPolypoint()
{
}

inline const i2d::CVector2d& CPolypoint::GetPoint(int index) const
{
	Q_ASSERT(index >= 0 && index < int(m_points.size()));

	return m_points[index];
}

inline int CPolypoint::GetPointsCount() const
{
	return int(m_points.size());
}

inline void CPolypoint::Insert(const i2d::CVector2d& vector)
{
	m_points.push_back(vector);
}


inline void CPolypoint::Clear()
{
	m_points.clear();
}


inline const QVector<i2d::CVector2d>& CPolypoint::GetPoints() const
{
	return m_points;
}


} // namespace i2d


#endif // !i2d_CPolypoint_included

