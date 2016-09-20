#ifndef iview_CScreenTransform_included
#define iview_CScreenTransform_included


#include <i2d/CAffine2d.h>


namespace iview
{


class CScreenTransform: public i2d::CAffine2d
{
public:
	typedef i2d::CAffine2d BaseClass;

	/**
		Default constructor.
		Note, that members will be not initialize.
	*/
	CScreenTransform();
	
	/**
		Copy constructor.
	*/
	CScreenTransform(const CScreenTransform& transform);
	
	/**
		Constructs CScreenTransform using i2d::CAffine2d object.
	*/
	CScreenTransform(const i2d::CAffine2d& transform);

	/**
		Return position of point in logical coordinates.
	*/
	i2d::CVector2d GetClientPosition(istd::CIndex2d position) const;
	
	/**
		Return position of point in screen coordinates.
	*/
	istd::CIndex2d GetScreenPosition(const i2d::CVector2d& position) const;
	
	/**
		Return reference to invert transformation
	*/
	const i2d::CAffine2d& GetInverted() const;

	// reimplemented (i2d::CAffine2d)
	void Reset();
	void Reset(const i2d::CVector2d& position);
	void Reset(const i2d::CVector2d& position, double angle, double scale = 1.0);
	void Reset(const i2d::CVector2d& position, double angle, const i2d::CVector2d& scale);
	using BaseClass::GetApply;
	void GetApply(const i2d::CAffine2d& transform, CScreenTransform& result) const;
	void Apply(const i2d::CAffine2d& transform);
	void ApplyLeft(const i2d::CAffine2d& transform);
	i2d::CAffine2d& operator=(const i2d::CAffine2d& transform);

protected:
	void CalculateInvert();

private:
	i2d::CAffine2d m_invert;
};


inline CScreenTransform::CScreenTransform()
{
}


inline CScreenTransform::CScreenTransform(const CScreenTransform& transform)
	:	i2d::CAffine2d(transform),
		m_invert(transform.m_invert)
{
}


inline CScreenTransform::CScreenTransform(const i2d::CAffine2d& transform)
	:	i2d::CAffine2d(transform)
{
	CalculateInvert();
}


inline i2d::CVector2d CScreenTransform::GetClientPosition(istd::CIndex2d position) const
{
	i2d::CVector2d result;
	i2d::CVector2d pos(position.GetX(), position.GetY());

	m_invert.GetApply(pos, result);

	return result;
}


inline istd::CIndex2d CScreenTransform::GetScreenPosition(const i2d::CVector2d& position) const
{
	i2d::CVector2d result;
	GetApply(position, result);

	return istd::CIndex2d(int(result.GetX()), int(result.GetY()));
}


// reimplemented (i2d::CAffine2d)

inline void CScreenTransform::Reset()
{
	BaseClass::Reset();

	CalculateInvert();
}


inline void CScreenTransform::Reset(const i2d::CVector2d& position)
{
	BaseClass::Reset(position);

	CalculateInvert();
}


inline void CScreenTransform::Reset(const i2d::CVector2d& position, double angle, double scale)
{
	BaseClass::Reset(position, angle, scale);

	CalculateInvert();
}


inline void CScreenTransform::Reset(const i2d::CVector2d& position, double angle, const i2d::CVector2d& scale)
{
	BaseClass::Reset(position, angle, scale);

	CalculateInvert();
}


inline void CScreenTransform::Apply(const i2d::CAffine2d& transform)
{
	BaseClass::Apply(transform);

	CalculateInvert();
}


inline i2d::CAffine2d& CScreenTransform::operator=(const i2d::CAffine2d& transform)
{
	BaseClass::operator=(transform);

	CalculateInvert();

	return *this;
}


inline void CScreenTransform::CalculateInvert()
{
	BaseClass::GetInverted(m_invert);
}


inline const i2d::CAffine2d& CScreenTransform::GetInverted() const
{
	return m_invert;
}


} // namespace iview


#endif // !iview_CScreenTransform_included


