#ifndef icmm_CRgb_included
#define icmm_CRgb_included


#include "iser/IArchive.h"

#include "icmm/TComposedColor.h"


namespace icmm
{


/**
	Primitive for representation of RGB color value.
	Please note, that normal components values are in range [0, 1].
*/
class CRgb: public TComposedColor<3>
{
public:
	typedef TComposedColor<3> BaseClass;

	enum ComponentIndex
	{
		Red,
		Green,
		Blue,
		Last = Blue
	};

	CRgb(double r = 0.0, double g = 0.0, double b = 0.0);
	CRgb(const CRgb& color);

	// access to component values
	double GetRed() const;
	void SetRed(double value);
	double GetGreen() const;
	void SetGreen(double value);
	double GetBlue() const;
	void SetBlue(double value);

	bool Serialize(iser::IArchive& archive);

	// operators
	CRgb operator+(const CRgb& color) const;
	CRgb operator-(const CRgb& color) const;
	CRgb operator*(const CRgb& color) const;
	CRgb operator/(const CRgb& color) const;

	CRgb operator*(double value) const;
	CRgb operator/(double value) const;

	const CRgb& operator=(const CRgb& color);

	const CRgb& operator+=(const CRgb& color);
	const CRgb& operator-=(const CRgb& color);
	const CRgb& operator*=(const CRgb& color);
	const CRgb& operator/=(const CRgb& color);

	const CRgb& operator*=(double value);
	const CRgb& operator/=(double value);
};


// inline methods

inline CRgb::CRgb(double r, double g, double b)
{
	SetComponentValue(Red, r);
	SetComponentValue(Green, g);
	SetComponentValue(Blue, b);
}


inline CRgb::CRgb(const CRgb& color)
:	BaseClass(color)
{
}


// access to components

inline double CRgb::GetRed() const
{
	return GetComponentValue(Red);
}


inline void CRgb::SetRed(double value)
{
	SetComponentValue(Red, value);
}


inline double CRgb::GetGreen() const
{
	return GetComponentValue(Green);
}


inline void CRgb::SetGreen(double value)
{
	SetComponentValue(Green, value);
}


inline double CRgb::GetBlue() const
{
	return GetComponentValue(Blue);
}


inline void CRgb::SetBlue(double value)
{
	SetComponentValue(Blue, value);
}


// operators

inline const CRgb& CRgb::operator=(const CRgb& color)
{
	BaseClass::operator=(color);

	return *this;
}


inline const CRgb& CRgb::operator+=(const CRgb& color)
{
	BaseClass::operator+=(color);

	return *this;
}


inline const CRgb& CRgb::operator-=(const CRgb& color)
{
	BaseClass::operator-=(color);

	return *this;
}


inline const CRgb& CRgb::operator*=(const CRgb& color)
{
	BaseClass::operator*=(color);

	return *this;
}


inline const CRgb& CRgb::operator/=(const CRgb& color)
{
	BaseClass::operator/=(color);

	return *this;
}


inline const CRgb& CRgb::operator*=(double value)
{
	BaseClass::operator*=(value);

	return *this;
}


inline const CRgb& CRgb::operator/=(double value)
{
	BaseClass::operator/=(value);

	return *this;
}


} // namespace icmm


#endif // !icmm_CRgb_included


