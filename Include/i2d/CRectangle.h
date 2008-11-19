#ifndef i2d_CRectangle_included
#define	i2d_CRectangle_included


#include "istd/CRange.h"
#include "istd/CIndex2d.h"

#include "iser/ISerializable.h"

#include "i2d/IObject2d.h"


namespace i2d
{


class CRectangle: virtual public IObject2d
{
public :
	CRectangle();
	CRectangle(const CRectangle& rect);
	CRectangle(double left, double top, double width, double height);
	CRectangle(const CVector2d& topLeft, const CVector2d& bottomRight);
	CRectangle(const istd::CRange& horizontalRange, const istd::CRange& verticalRange);
	explicit CRectangle(const istd::CIndex2d& size);

	bool IsValid() const;
	bool IsEmpty() const;

	double GetLeft() const;
	void SetLeft(double left);
	double GetTop() const;
	void SetTop(double top);
	double GetRight() const;
	void SetRight(double right);
	double GetBottom() const;
	void SetBottom(double bottom);

	const istd::CRange& GetHorizontalRange() const;
	istd::CRange& GetHorizontalRangeRef();
	void SetHorizontalRange(const istd::CRange& range);

	const istd::CRange& GetVerticalRange() const;
	istd::CRange& GetVerticalRangeRef();
	void SetVerticalRange(const istd::CRange& range);

	CVector2d GetTopLeft() const;
	void SetTopLeft(const CVector2d& topLeft);
	CVector2d GetTopRight() const;
	void SetTopRight(const CVector2d& topRight);
	CVector2d GetBottomLeft() const;
	void SetBottomLeft(const CVector2d& bottomLeft);
	CVector2d GetBottomRight() const;
	void SetBottomRight(const CVector2d& bottomRight);

	bool Contains(const CVector2d& point) const;
	bool Contains(const istd::CIndex2d& point) const;
	bool Contains(const CRectangle& rect) const;
	double GetWidth() const;
	double GetHeight() const;

	bool IsIntersectedBy(const CRectangle& rect) const;
	CRectangle GetIntersection(const CRectangle& rect) const;
	void Intersection(const CRectangle& rect);

	CRectangle GetUnion(const CRectangle& rect) const;
	void Union(const CRectangle& rect);

	CRectangle GetExpanded(const CRectangle& rect) const;
	void Expand(const CRectangle& rect);

	const CRectangle& operator=(const CRectangle& rect);

	// reimplemented (IObject2d)
	virtual CVector2d GetCenter() const;
	virtual CRectangle GetBoundingBox() const;
	virtual void MoveTo(const CVector2d& position);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	istd::CRange m_horizontalRange;
	istd::CRange m_verticalRange;
};


// inline methods
	
inline double CRectangle::GetLeft() const
{
	return m_horizontalRange.GetMinValue();
}


inline double CRectangle::GetTop() const
{
	return m_verticalRange.GetMinValue();
}


inline double CRectangle::GetRight() const
{
	return m_horizontalRange.GetMaxValue();
}


inline double CRectangle::GetBottom() const
{
	return m_verticalRange.GetMaxValue();
}


inline const istd::CRange& CRectangle::GetHorizontalRange() const
{
	return m_horizontalRange;
}


inline istd::CRange& CRectangle::GetHorizontalRangeRef()
{
	return m_horizontalRange;
}


inline const istd::CRange& CRectangle::GetVerticalRange() const
{
	return m_verticalRange;
}


inline istd::CRange& CRectangle::GetVerticalRangeRef()
{
	return m_verticalRange;
}


} // namespace i2d


#endif // !i2d_CRectangle_included


