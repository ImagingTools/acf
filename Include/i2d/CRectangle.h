#ifndef i2d_CRectangle_included
#define i2d_CRectangle_included


// Qt includes
#include <QtCore/QRectF>

// ACF includes
#include "istd/TRange.h"
#include "istd/CIndex2d.h"
#include "iser/ISerializable.h"
#include "i2d/CObject2dBase.h"
#include "i2d/CLine2d.h"


namespace i2d
{


class CRect;


/**
	Definition of rectangle area orthogonal to axis of coordination system.
*/
class CRectangle: public CObject2dBase
{
public:
	CRectangle();
	CRectangle(double left, double top, double width, double height);
	CRectangle(const CVector2d& topLeft, const CVector2d& bottomRight);
	CRectangle(const istd::CRange& horizontalRange, const istd::CRange& verticalRange);
	CRectangle(const i2d::CRect& rect);
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
		Return \c true if the rectangle is valid and it is not empty.
	*/
	bool IsValidNonEmpty() const;

	/**
		Make rectangle empty.
	*/
	void Reset();

	/**
		Get value of left boundary.
	*/
	double GetLeft() const;
	/**
		Set value of left boundary.
	*/
	void SetLeft(double left);
	/**
		Get value of top boundary.
	*/
	double GetTop() const;
	/**
		Set value of top boundary.
	*/
	void SetTop(double top);
	/**
		Get value of right boundary.
	*/
	double GetRight() const;
	/**
		Set value of right boundary.
	*/
	void SetRight(double right);
	/**
		Get value of bottom boundary.
	*/
	double GetBottom() const;
	/**
		Set value of bottom boundary.
	*/
	void SetBottom(double bottom);

	/**
		Get values of horizontal boundaries.
	*/
	const istd::CRange& GetHorizontalRange() const;
	/**
		Get values of horizontal boundaries as reference.
		Please use update notificators (like istd::CChangeNotifier) to be sure that obervers will be updated correctly.
	*/
	istd::CRange& GetHorizontalRangeRef();
	/**
		Set values of horizontal boundaries.
	*/
	void SetHorizontalRange(const istd::CRange& range);

	/**
		Get values of vertical boundaries.
	*/
	const istd::CRange& GetVerticalRange() const;
	/**
		Get values of vertical boundaries as reference.
		Please use update notificators (like istd::CChangeNotifier) to be sure that obervers will be updated correctly.
	*/
	istd::CRange& GetVerticalRangeRef();
	/**
		Set values of vertical boundaries.
	*/
	void SetVerticalRange(const istd::CRange& range);

	/**
		Get left-top point of this rectangle.
	*/
	CVector2d GetLeftTop() const;
	/**
		Set left-top point of this rectangle.
	*/
	void SetTopLeft(const CVector2d& topLeft);
	/**
		Get right-top point of this rectangle.
	*/
	CVector2d GetRightTop() const;
	/**
		Set right-top point of this rectangle.
	*/
	void SetTopRight(const CVector2d& topRight);
	/**
		Get left-bottom point of this rectangle.
	*/
	CVector2d GetLeftBottom() const;
	/**
		Set left-bottom point of this rectangle.
	*/
	void SetBottomLeft(const CVector2d& bottomLeft);
	/**
		Get right-bottom point of this rectangle.
	*/
	CVector2d GetRightBottom() const;
	/**
		Set right-bottom point of this rectangle.
	*/
	void SetBottomRight(const CVector2d& bottomRight);

	/**
		Check if some point is contained in this rectangle.
	*/
	bool Contains(const CVector2d& point) const;
	/**
		Check if some point is contained in this rectangle.
	*/
	bool Contains(const istd::CIndex2d& point) const;
	/**
		Check if some rectangle is fully contained in this rectangle.
		The calibration will be ignored.
	*/
	bool Contains(const CRectangle& rect) const;

	/**
		Get width of this rectangle.
	*/
	double GetWidth() const;
	/**
		Get height of this rectangle.
	*/
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
		Rectangle is intersected, if there is any point belonging to line and rectangle (filled).
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

	/**
		Get union of two rectangles.
		Union of rectangles is the smallest rectangle containing both.
		The calibration will be ignored.
	*/
	CRectangle GetUnion(const CRectangle& rect) const;
	/**
		Set this rectangle to union of two rectangles.
		The calibration will be ignored.
	*/
	void Unite(const CRectangle& rect);

	/**
		Get union of this rectangle and point.
		This is the smallest rectangle containing both elements.
	*/
	CRectangle GetUnion(const i2d::CVector2d& position) const;
	/**
		Set this rectangle to union of rectangle and point.
		This union is the smallest rectangle containing both elements.
	*/
	void Unite(const i2d::CVector2d& position);

	/**
		Get expanded rectangle (simple sum of boundary values).
	*/
	CRectangle GetExpanded(const CRectangle& rect) const;
	/**
		Expand rectangle.
		This expanded rectangle is simple sum of boundary values.
	*/
	void Expand(const CRectangle& rect);

	/**
		Get distance from nearest point of this rectangle to specified position.
	*/
	double GetDistance(const CVector2d& position) const;

	/**
		Get maximal distance from points of this rectangle to specified position.
	*/
	double GetMaxDistance(const CVector2d& position) const;

	/**
		Get the nearest point in the rectangle for the some given point.
	*/
	CVector2d GetNearestPointTo(const CVector2d& point) const;

	void Translate(const i2d::CVector2d& delta); 
	CRectangle GetTranslated(const i2d::CVector2d& delta) const;
	
	bool operator==(const CRectangle& rect) const;
	bool operator!=(const CRectangle& rect) const;

	operator QRectF() const;

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
	virtual bool CopyFrom(const IChangeable& object, CompatibilityMode mode = CM_WITHOUT_REFS);
	virtual istd::IChangeable* CloneMe(CompatibilityMode mode = CM_WITHOUT_REFS) const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

	// static methods
	static const i2d::CRectangle& GetEmpty();

private:
	istd::CRange m_horizontalRange;
	istd::CRange m_verticalRange;

	// static atributes
	static CRectangle s_empty;
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


// static methods

inline const i2d::CRectangle& CRectangle::GetEmpty()
{
	return s_empty;
}


} // namespace i2d


#endif // !i2d_CRectangle_included


