#ifndef i2d_CSplineSegment_included
#define i2d_CSplineSegment_included


// ACF includes
#include "istd/IChangeable.h"

#include "i2d/CVector2d.h"


namespace i2d
{


class CSplineSegment: virtual public istd::IChangeable
{
public:
	/**
		Get point.
		\param	position Position in segment. Must be in range [0, 1], where 0 is begin, and 1 is end of segment.
	*/
	virtual i2d::CVector2d GetPointInSegment(double position) const;

	/**
		Get derivation.
		\param	position Position in segment. Must be in range [0, 1], where 0 is begin, and 1 is end of segment.
	*/
	virtual i2d::CVector2d GetDerivationInSegment(double position) const;
	
	/**
		Get derivation in begin point.
	*/
	virtual i2d::CVector2d GetDerivationBegin() const;

	/**
		Get derivation in end point.
	*/
	virtual i2d::CVector2d GetDerivationEnd() const;

	/**
		Get begin point.
	*/
	virtual i2d::CVector2d GetPointBegin() const;

	/**
		Get end point.
	*/
	virtual i2d::CVector2d GetPointEnd() const;
	
	/**
		Get first Bezier's control point.
	*/
	virtual i2d::CVector2d GetBezierPointBegin() const;
	
	/**
		Get second Bezier's control point.
	*/
	virtual i2d::CVector2d GetBezierPointEnd() const;
	
	/**
		Check, if segment is closer than specifier distance.
	*/
	virtual bool IsCloserThan(const i2d::CVector2d position, double distance) const;
	
	/**
		Get a first half segment.
	*/
	virtual void GetHalfSegmentFirst(CSplineSegment& result) const;
	
	/**
		Get a second half segment.
	*/
	virtual void GetHalfSegmentSecond(CSplineSegment& result) const;

	// acces to members
	const i2d::CVector2d& A() const;
	void A(const i2d::CVector2d& value);
	const i2d::CVector2d& B() const;
	void B(const i2d::CVector2d& value);
	const i2d::CVector2d& C() const;
	void C(const i2d::CVector2d& value);
	const i2d::CVector2d& D() const;
	void D(const i2d::CVector2d& value);

	//reimplemented istd::IChangeable
	virtual int GetSupportedOperations() const;
	virtual bool CopyFrom(const IChangeable& object, CompatibilityMode mode = CM_WITHOUT_REFS);
	virtual istd::IChangeable* CloneMe(CompatibilityMode mode = CM_WITHOUT_REFS) const;

private:
	i2d::CVector2d m_a;
	i2d::CVector2d m_b;
	i2d::CVector2d m_c;
	i2d::CVector2d m_d;
};


inline i2d::CVector2d CSplineSegment::GetPointInSegment(double position) const
{
	return ((m_a * position + m_b) * position + m_c) * position + m_d;
}


inline i2d::CVector2d CSplineSegment::GetDerivationInSegment(double position) const
{
	return (m_a * (3 * position) + m_b * 2) * position + m_c;
}


inline i2d::CVector2d CSplineSegment::GetDerivationBegin() const
{
	return m_c;
}


inline i2d::CVector2d CSplineSegment::GetDerivationEnd() const
{
	return m_a * 3 + m_b * 2 + m_c;
}


inline i2d::CVector2d CSplineSegment::GetPointBegin() const
{
	return m_d;
}


inline i2d::CVector2d CSplineSegment::GetPointEnd() const
{
	return m_a + m_b + m_c + m_d;
}


inline i2d::CVector2d CSplineSegment::GetBezierPointBegin() const
{
	return m_d + m_c / 3;
}


inline i2d::CVector2d CSplineSegment::GetBezierPointEnd() const
{
	return m_c + m_d + (m_b - m_c) / 3;
}


inline void CSplineSegment::GetHalfSegmentFirst(CSplineSegment& result) const
{
	result.m_a = m_a * 0.125;
	result.m_b = m_b * 0.25;
	result.m_c = m_c * 0.5;
	result.m_d = m_d;
}


inline void CSplineSegment::GetHalfSegmentSecond(CSplineSegment& result) const
{
	result.m_a = m_a * 0.125;
	result.m_b = m_a * 0.375 + m_b * 0.25;
	result.m_c = m_a * 0.375 + (m_b + m_c) * 0.5;
	result.m_d = m_a * 0.125 + m_b * 0.25 + m_c * 0.5 + m_d;
}


// acces to members

inline const i2d::CVector2d& CSplineSegment::A() const
{
	return m_a;
}


inline void CSplineSegment::A(const i2d::CVector2d& value)
{
	m_a = value;
}


inline const i2d::CVector2d& CSplineSegment::B() const
{
	return m_b;
}


inline void CSplineSegment::B(const i2d::CVector2d& value)
{
	m_b = value;
}


inline const i2d::CVector2d& CSplineSegment::C() const
{
	return m_c;
}


inline void CSplineSegment::C(const i2d::CVector2d& value)
{
	m_c = value;
}


inline const i2d::CVector2d& CSplineSegment::D() const
{
	return m_d;
}


inline void CSplineSegment::D(const i2d::CVector2d& value)
{
	m_d = value;
}


} // namespace i2d


#endif // !i2d_CSplineSegment_included
