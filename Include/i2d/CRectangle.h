#ifndef i2d_CRectangle_included
#define i2d_CRectangle_included


#include "istd/CRange.h"
#include "istd/CIndex2d.h"

#include "iser/ISerializable.h"

#include "i2d/IObject2d.h"
#include "i2d/CLine2d.h"


namespace i2d
{


/**
	Definition of rectangle area orthogonal to axis of coordination system.
*/
class CRectangle: virtual public IObject2d
{
public:
	CRectangle();
	CRectangle(const CRectangle& rect);
	CRectangle(double left, double top, double width, double height);
	CRectangle(const CVector2d& topLeft, const CVector2d& bottomRight);
	CRectangle(const istd::CRange& horizontalRange, const istd::CRange& verticalRange);
	explicit CRectangle(const istd::CIndex2d& size);

	/**
		Return \c true if the rectangle is valid.
	*/
	bool IsValid() const;

	/**
		Return \c true if the rectangle is empty.
	*/
	bool IsEmpty() const;

	/**
		Make rectangle empty.
	*/
	void Reset();

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

	/**
		Get size of this rectangle.
	*/
	CVector2d GetSize() const;

	/**
		Returns \c true, if this rectangle is intersected by the other rectangle \c rect.
	*/
	bool IsIntersectedBy(const CRectangle& rect) const;		

	/**
		Returns \c true, if this rectangle is intersected by the line \c rect.
	*/
	bool IsIntersectedBy(const CLine2d& line) const;

	/**
		Get resulting rectangle of the intersection of this rectangle with \c rect.
	*/
	CRectangle GetIntersection(const CRectangle& rect) const;

	/**
		Get resulting line of the intersection of this rectangle with the line \c line.
	*/
	CLine2d GetIntersection(const CLine2d& line) const;

	/**
		Set new rectangle as intersection of the current rectangle with a given one.
	*/
	void Intersect(const CRectangle& rectangle);

	CRectangle GetUnion(const CRectangle& rect) const;
	void Unite(const CRectangle& rect);

	CRectangle GetUnion(const i2d::CVector2d& position) const;
	void Unite(const i2d::CVector2d& position);

	CRectangle GetExpanded(const CRectangle& rect) const;
	void Expand(const CRectangle& rect);

	const CRectangle& operator=(const CRectangle& rect);
	bool operator==(const CRectangle& rect) const;
	bool operator!=(const CRectangle& rect) const;

	CRectangle GetBoundingBox() const;

	/**
		Get the nearest point in the rectangle for the some given point.
	*/
	CVector2d GetNearestPointTo(const CVector2d& point) const;

	void Translate(const i2d::CVector2d& delta); 
	CRectangle GetTranslated(const i2d::CVector2d& delta) const;
	
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


inline void CRectangle::Translate(const i2d::CVector2d& delta)
{
	m_horizontalRange.SetMinValue(m_horizontalRange.GetMinValue() + delta.GetX());
	m_verticalRange.SetMinValue(m_verticalRange.GetMinValue() + delta.GetY());
	m_horizontalRange.SetMaxValue(m_horizontalRange.GetMaxValue() + delta.GetX());
	m_verticalRange.SetMaxValue(m_verticalRange.GetMaxValue() + delta.GetY());
}


} // namespace i2d


#endif // !i2d_CRectangle_included


