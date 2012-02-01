#ifndef i2d_CRect_included
#define i2d_CRect_included


// ACF includes
#include "istd/CIndex2d.h"
#include "istd/CIntRange.h"

#include "i2d/CRectangle.h"

#include "ibase/CSize.h"


namespace i2d
{


/**
	Simple rectangle with integer bounds.
*/
class CRect
{
public:
	CRect();
	CRect(const CRect& rect);
	CRect(int left, int top, int right, int bottom);
	CRect(const istd::CIndex2d& leftTop, const istd::CIndex2d& rightBottom);
	CRect(const istd::CIndex2d& leftTop, const ibase::CSize& size);
	CRect(const i2d::CRectangle& rect);
	explicit CRect(const ibase::CSize& size);

	i2d::CRectangle GetRectangle() const;

	istd::CIndex2d GetLeftTop() const;
	void SetLeftTop(istd::CIndex2d position);
	istd::CIndex2d GetRightTop() const;
	void SetRightTop(istd::CIndex2d position);
	istd::CIndex2d GetLeftBottom() const;
	void SetLeftBottom(istd::CIndex2d position);
	istd::CIndex2d GetRightBottom() const;
	void SetRightBottom(istd::CIndex2d position);

	int GetLeft() const;
	void SetLeft(int left);

	int GetTop() const;
	void SetTop(int top);

	int GetRight() const;
	void SetRight(int right);

	int GetBottom() const;
	void SetBottom(int bottom);

	int GetWidth() const;
	int GetHeight() const;

	istd::CIntRange GetHorizontalRange() const;
	istd::CIntRange GetVerticalRange() const;

	istd::CIndex2d GetCenter() const;

	/**
		Check if rectangle is valid.
	*/
	bool IsValid() const;
	/**
		Check if rectangle is empty.
	*/
	bool IsEmpty() const;
	/**
		Check if all parameters are 0.
	*/
	bool IsNull() const;
	/**
		Get size of rectangle.
	*/
	ibase::CSize GetSize() const;
	/**
		Check if specified point lies inside.
	*/
	bool IsInside(const istd::CIndex2d& point) const;
	/**
		Check if specified rectangle lies inside.
	*/
	bool IsInside(const CRect& rect) const;
	/**
		Check if specified rectangle lies fully outside.
	*/
	bool IsOutside(const CRect& rect) const;
	/**
		Set all members to 0.
	*/
	void Reset();
	/**
		Calculate union of this and second rectangle and stores result in this object.
	*/
	void Union(const CRect& rect);
	/**
		Get union of two rectangles.
	*/
	void GetUnion(const CRect& rect, CRect& result) const;
	/**
		Get union of two rectangles.
		\overload
	*/
	CRect GetUnion(const CRect& rect) const;
	/**
		Calculate union of this rectangle and point.
		Result is atored in this object.
	*/
	void Union(istd::CIndex2d point);
	/**
		Get union of this rectangle and point.
	*/
	CRect GetUnion(istd::CIndex2d point) const;
	/**
		Expand rectangle using second rectangle.
		Expanded rectangle is easy sum of coordinates of both rectangles.
	*/
	void Expand(const CRect& rect);
	/**
		Get expanded rectangle.
		Expanded rectangle is easy sum of coordinates of both rectangles.
	*/
	CRect GetExpanded(const CRect& rect) const;
	/**
		Calculate intersection of this and second rectangle and stores result in this object.
	*/
	void Intersection(const CRect& rect);
	/**
		Get intersection of two rectangles.
	*/
	CRect GetIntersection(const CRect& rect) const;
	/**
		Move rectangle.
	*/
	void Translate(istd::CIndex2d point);
	/**
		Get moved rectangle.
	*/
	CRect GetTranslated(istd::CIndex2d point) const;

	// operators
	CRect& operator=(const CRect& rect);
	bool operator==(const CRect& rect) const;
	bool operator!=(const CRect& rect) const;

	// static methods
	static const i2d::CRect& GetEmpty();

private:
	// static atributes
	static CRect s_empty;

	istd::CIntRange m_horizontalRange;
	istd::CIntRange m_verticalRange;
};


// inline methods

inline CRect::CRect()
{
#ifdef _DEBUG
	//	set to be invalid
	m_horizontalRange = istd::CIntRange::GetInvalid();
	m_verticalRange = istd::CIntRange::GetInvalid();
#endif //_DEBUG
}


inline CRect::CRect(const CRect& rect)
{
	*this = rect;
}


inline CRect::CRect(int left, int top, int right, int bottom)
{
	m_horizontalRange = istd::CIntRange(left, right);
	m_verticalRange = istd::CIntRange(top, bottom);

	I_ASSERT(IsValid());
}


inline CRect::CRect(const istd::CIndex2d& leftTop, const istd::CIndex2d& rightBottom)
{
	m_horizontalRange = istd::CIntRange(leftTop.GetX(), rightBottom.GetX());
	m_verticalRange = istd::CIntRange(leftTop.GetY(), rightBottom.GetY());

	I_ASSERT(IsValid());
}


inline CRect::CRect(const istd::CIndex2d& leftTop, const ibase::CSize& size)
{
	m_horizontalRange = istd::CIntRange(leftTop.GetX(), leftTop.GetX() + size.GetX());
	m_verticalRange = istd::CIntRange(leftTop.GetY(), leftTop.GetY() + size.GetY());

	I_ASSERT(IsValid());
}


inline CRect::CRect(const i2d::CRectangle& rect)
{
	m_horizontalRange = istd::CIntRange(int(rect.GetLeft()), int(rect.GetRight()));
	m_verticalRange = istd::CIntRange(int(rect.GetTop()), int(rect.GetBottom()));

	I_ASSERT(IsValid());
}


inline CRect::CRect(const ibase::CSize& size)
{
	m_horizontalRange = istd::CIntRange(0, size.GetX());
	m_verticalRange = istd::CIntRange(0, size.GetY());

	I_ASSERT(IsValid());
}


inline i2d::CRectangle CRect::GetRectangle() const
{
	return i2d::CRectangle(
				m_horizontalRange.GetMinValue(),
				m_verticalRange.GetMinValue(),
				m_horizontalRange.GetLength(),
				m_verticalRange.GetLength());
}


inline istd::CIndex2d CRect::GetLeftTop() const
{
	return istd::CIndex2d(m_horizontalRange.GetMinValue(), m_verticalRange.GetMinValue());
}


inline void CRect::SetLeftTop(istd::CIndex2d position)
{
	m_horizontalRange.SetMinValue(position.GetX());
	m_verticalRange.SetMinValue(position.GetY());
}


inline istd::CIndex2d CRect::GetRightTop() const
{
	return istd::CIndex2d(m_horizontalRange.GetMaxValue(), m_verticalRange.GetMinValue());
}


inline void CRect::SetRightTop(istd::CIndex2d position)
{
	m_horizontalRange.SetMaxValue(position.GetX());
	m_verticalRange.SetMinValue(position.GetY());
}


inline istd::CIndex2d CRect::GetLeftBottom() const
{
	return istd::CIndex2d(m_horizontalRange.GetMinValue(), m_verticalRange.GetMaxValue());
}


inline void CRect::SetLeftBottom(istd::CIndex2d position)
{
	m_horizontalRange.SetMinValue(position.GetX());
	m_verticalRange.SetMaxValue(position.GetY());
}


inline istd::CIndex2d CRect::GetRightBottom() const
{
	return istd::CIndex2d(m_horizontalRange.GetMaxValue(), m_verticalRange.GetMaxValue());
}


inline void CRect::SetRightBottom(istd::CIndex2d position)
{
	m_horizontalRange.SetMaxValue(position.GetX());
	m_verticalRange.SetMaxValue(position.GetY());
}


inline int CRect::GetLeft() const
{
	return m_horizontalRange.GetMinValue();
}


inline int CRect::GetTop() const
{
	return m_verticalRange.GetMinValue();
}


inline int CRect::GetRight() const
{
	return m_horizontalRange.GetMaxValue();
}


inline int CRect::GetBottom() const
{
	return m_verticalRange.GetMaxValue();

}

inline int CRect::GetWidth() const
{
	return m_horizontalRange.GetLength();
}


inline int CRect::GetHeight() const
{
	return m_verticalRange.GetLength();
}


inline istd::CIntRange CRect::GetHorizontalRange() const
{
	return m_horizontalRange;
}


inline istd::CIntRange CRect::GetVerticalRange() const
{
	return m_verticalRange;
}


inline istd::CIndex2d CRect::GetCenter() const
{
	return istd::CIndex2d(
			m_horizontalRange.GetMinValue() + m_horizontalRange.GetLength() / 2,
			m_verticalRange.GetMinValue() + m_verticalRange.GetLength() / 2);
}


inline bool CRect::IsValid() const
{
	return (m_horizontalRange.IsValid()) && (m_verticalRange.IsValid());
}


inline bool CRect::IsEmpty() const
{
	return (m_horizontalRange.IsEmpty()) || (m_verticalRange.IsEmpty());
}


inline bool CRect::IsNull() const
{
	return (m_horizontalRange == istd::CIntRange::GetNull()) && (m_verticalRange == istd::CIntRange::GetNull());
}


inline ibase::CSize CRect::GetSize() const
{
	return ibase::CSize(m_horizontalRange.GetLength(), m_verticalRange.GetLength());
}


inline bool CRect::IsInside(const istd::CIndex2d& point) const
{
	I_ASSERT(IsValid());

	return m_horizontalRange.Contains(point.GetX()) && m_verticalRange.Contains(point.GetY());
}


inline bool CRect::IsInside(const CRect& rect) const
{
	I_ASSERT(IsValid());
	I_ASSERT(rect.IsValid());

	if (rect.IsEmpty()){
		return true;
	}
	else{
		return m_horizontalRange.Contains(rect.m_horizontalRange) && m_verticalRange.Contains(rect.m_verticalRange);
	}
}


inline bool CRect::IsOutside(const CRect& rect) const
{
	I_ASSERT(IsValid());
	I_ASSERT(rect.IsValid());

	if (rect.IsEmpty()){
		return true;
	}
	else{
		return m_horizontalRange.IsOutsideOf(rect.m_horizontalRange) || m_verticalRange.IsOutsideOf(rect.m_verticalRange);
	}
}


inline void CRect::Reset()
{
	m_horizontalRange.Reset();
	m_verticalRange.Reset();
}


inline void CRect::Union(const CRect& rect)
{
	I_ASSERT(rect.IsValid());
	I_ASSERT(IsValid());

	if (IsEmpty()){
		*this = rect;
	}
	else if (!rect.IsEmpty()){
		m_horizontalRange.Unite(rect.m_horizontalRange);
		m_verticalRange.Unite(rect.m_verticalRange);
	}
}


inline void CRect::GetUnion(const CRect& rect, CRect& result) const
{
	I_ASSERT(rect.IsValid());
	I_ASSERT(IsValid());

	if (IsEmpty()){
		result = rect;
	}
	else if (!rect.IsEmpty()){
		result = *this;

		result.m_horizontalRange.Unite(rect.m_horizontalRange);
		result.m_verticalRange.Unite(rect.m_verticalRange);
	}
	else{
		result.Reset();
	}
}


inline CRect CRect::GetUnion(const CRect& rect) const
{
	I_ASSERT(rect.IsValid());
	I_ASSERT(IsValid());

	CRect result;

	GetUnion(rect, result);

	return result;
}


inline void CRect::Union(istd::CIndex2d point)
{
	I_ASSERT(IsValid());

	int left = istd::Min(GetLeft(), point.GetX());
	int top = istd::Min(GetTop(), point.GetY());
	int right = istd::Max(GetRight(), point.GetX());
	int bottom = istd::Max(GetBottom(), point.GetY());

	m_horizontalRange = istd::CIntRange(left, right);
	m_verticalRange = istd::CIntRange(top, bottom);
}


inline CRect CRect::GetUnion(istd::CIndex2d point) const
{
	I_ASSERT(IsValid());

	CRect result = *this;

	result.Union(point);

	return result;
}


inline void CRect::Expand(const CRect& rect)
{
	I_ASSERT(rect.IsValid());
	I_ASSERT(IsValid());

	m_horizontalRange.Expand(rect.m_horizontalRange);
	m_verticalRange.Expand(rect.m_verticalRange);
}


inline CRect CRect::GetExpanded(const CRect& rect) const
{
	I_ASSERT(rect.IsValid());
	I_ASSERT(IsValid());

	CRect result = *this;

	result.Expand(rect);

	return result;
}


inline void CRect::Intersection(const CRect& rect)
{
	I_ASSERT(rect.IsValid());
	I_ASSERT(IsValid());

	m_horizontalRange.Intersection(rect.m_horizontalRange);
	m_verticalRange.Intersection(rect.m_verticalRange);

	if (!m_horizontalRange.IsValid() || !m_verticalRange.IsValid()){
		Reset();
	}
}


inline CRect CRect::GetIntersection(const CRect& rect) const
{
	I_ASSERT(rect.IsValid());
	I_ASSERT(IsValid());

	CRect result = *this;

	result.Intersection(rect);
	
	return result;
}


inline void CRect::Translate(istd::CIndex2d point)
{
	m_horizontalRange.SetMinValue(m_horizontalRange.GetMinValue() + point.GetX());
	m_horizontalRange.SetMaxValue(m_horizontalRange.GetMaxValue() + point.GetX());
	m_verticalRange.SetMinValue(m_verticalRange.GetMinValue() + point.GetY());
	m_verticalRange.SetMaxValue(m_verticalRange.GetMaxValue() + point.GetY());
}


inline CRect CRect::GetTranslated(istd::CIndex2d point) const
{
	I_ASSERT(IsValid());

	CRect result = *this;

	result.Translate(point);

	return result;
}


inline CRect& CRect::operator =(const CRect& rect)
{
	m_horizontalRange = rect.m_horizontalRange;
	m_verticalRange = rect.m_verticalRange;

	return *this;
}


inline bool CRect::operator==(const CRect& rect) const
{
	return (m_horizontalRange == rect.m_horizontalRange) && (m_verticalRange == rect.m_verticalRange);
}


inline bool CRect::operator!=(const CRect& rect) const
{
	return	!operator==(rect);
}


// static methods

inline const i2d::CRect& CRect::GetEmpty()
{
	return s_empty;
}


} // namespace i2d


#endif // !i2d_CRect_included


