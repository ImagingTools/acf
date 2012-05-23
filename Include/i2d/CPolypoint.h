#ifndef i2d_CPolypoint_included
#define i2d_CPolypoint_included


// Qt includes
#include <QtCore/QVector>

// ACF includes
#include "iser/CArchiveTag.h"

#include "i2d/IObject2d.h"
#include "i2d/CVector2d.h"


namespace i2d
{


class CPolypoint: virtual public IObject2d
{
public:
	CPolypoint();

	void Insert(const i2d::CVector2d& vector);
	void Clear();
	const QVector<i2d::CVector2d>& GetPoints() const;

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

private:
	typedef QVector<i2d::CVector2d> Points;

	Points m_points;
};


// inline methods

inline CPolypoint::CPolypoint()
{
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

