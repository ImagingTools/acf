#ifndef icmm_CLab_included
#define icmm_CLab_included


#include "iser/IArchive.h"

#include "icmm/TComposedColor.h"


namespace icmm
{


/**
	Primitive for representation of CMYK color value.
	Please note, that normal components values are in range [0, 1].
*/
class CLab: public TComposedColor<3>
{
public:
	typedef TComposedColor<3> BaseClass;

	enum ComponentIndex
	{
		CI_L,
		CI_A,
		CI_B
	};

	CLab(double l = 0.0, double a = 0.0, double b = 0.0);
	CLab(const CLab& color);

	// access to component values
	double GetL() const;
	void SetL(double value);
	double GetA() const;
	void SetA(double value);
	double GetB() const;
	void SetB(double value);

	bool Serialize(iser::IArchive& archive);

	// operators
	CLab operator+(const CLab& color) const;
	CLab operator-(const CLab& color) const;
	CLab operator*(const CLab& color) const;
	CLab operator/(const CLab& color) const;

	CLab operator*(double value) const;
	CLab operator/(double value) const;

	const CLab& operator=(const CLab& color);

	const CLab& operator+=(const CLab& color);
	const CLab& operator-=(const CLab& color);
	const CLab& operator*=(const CLab& color);
	const CLab& operator/=(const CLab& color);

	const CLab& operator*=(double value);
	const CLab& operator/=(double value);
};


// inline methods

inline CLab::CLab(double l, double a, double b)
{
	SetComponentValue(CI_L, l);
	SetComponentValue(CI_A, a);
	SetComponentValue(CI_B, b);
}


inline CLab::CLab(const CLab& color)
:	BaseClass(color)
{
}


// access to components

inline double CLab::GetL() const
{
	return GetComponentValue(CI_L);
}


inline void CLab::SetL(double value)
{
	SetComponentValue(CI_L, value);
}


inline double CLab::GetA() const
{
	return GetComponentValue(CI_A);
}


inline void CLab::SetA(double value)
{
	SetComponentValue(CI_A, value);
}


inline double CLab::GetB() const
{
	return GetComponentValue(CI_B);
}


inline void CLab::SetB(double value)
{
	SetComponentValue(CI_B, value);
}


// operators

inline const CLab& CLab::operator=(const CLab& color)
{
	BaseClass::operator=(color);

	return *this;
}


inline const CLab& CLab::operator+=(const CLab& color)
{
	BaseClass::operator+=(color);

	return *this;
}


inline const CLab& CLab::operator-=(const CLab& color)
{
	BaseClass::operator-=(color);

	return *this;
}


inline const CLab& CLab::operator*=(const CLab& color)
{
	BaseClass::operator*=(color);

	return *this;
}


inline const CLab& CLab::operator/=(const CLab& color)
{
	BaseClass::operator/=(color);

	return *this;
}


inline const CLab& CLab::operator*=(double value)
{
	BaseClass::operator*=(value);

	return *this;
}


inline const CLab& CLab::operator/=(double value)
{
	BaseClass::operator/=(value);

	return *this;
}


} // namespace icmm


#endif // !icmm_CLab_included


