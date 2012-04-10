#ifndef i2d_CPolypoint_included
#define i2d_CPolypoint_included


// Qt includes
#include <QtCore/QVector>

// ACF includes
#include "iser/CArchiveTag.h"
#include "iser/ISerializable.h"

#include "i2d/CVector2d.h"


namespace i2d
{


class CPolypoint: public iser::ISerializable
{
public:
	CPolypoint();

	void Insert(const i2d::CVector2d& vector);
	void Clear();
	const ::QVector<i2d::CVector2d>& GetPoints() const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	::QVector<i2d::CVector2d> m_points;
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


inline const ::QVector<i2d::CVector2d>& CPolypoint::GetPoints() const
{
	return m_points;
}


} // namespace i2d


#endif // !i2d_CPolypoint_included

