#pragma once


// ACF includes
#include <iser/IArchive.h>
#include <icmm/TComposedColor.h>


namespace icmm
{


/**
	Primitive for representation of LAB color value.
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

	virtual bool Serialize(iser::IArchive& archive) override;

	double GetDeltaE(const CLab& lab) const;

	// operators
	CLab operator+(const CLab& color) const;
	CLab operator-(const CLab& color) const;
	CLab operator*(const CLab& color) const;
	CLab operator/(const CLab& color) const;

	CLab operator*(double value) const;
	CLab operator/(double value) const;

	CLab& operator=(const CLab& color);

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
	SetElement(CI_L, l);
	SetElement(CI_A, a);
	SetElement(CI_B, b);
}


inline CLab::CLab(const CLab& color)
:	BaseClass(color)
{
}


// access to components

inline double CLab::GetL() const
{
	return GetElement(CI_L);
}


inline void CLab::SetL(double value)
{
	SetElement(CI_L, value);
}


inline double CLab::GetA() const
{
	return GetElement(CI_A);
}


inline void CLab::SetA(double value)
{
	SetElement(CI_A, value);
}


inline double CLab::GetB() const
{
	return GetElement(CI_B);
}


inline void CLab::SetB(double value)
{
	SetElement(CI_B, value);
}


// operators

inline CLab CLab::operator+(const CLab& color) const
{
	CLab retVal = *this;

	retVal += color;

	return retVal;
}


inline CLab CLab::operator-(const CLab& color) const
{
	CLab retVal = *this;

	retVal -= color;

	return retVal;
}


inline CLab CLab::operator*(const CLab& color) const
{
	CLab retVal = *this;

	retVal *= color;

	return retVal;
}


inline CLab CLab::operator/(const CLab& color) const
{
	CLab retVal = *this;

	retVal /= color;

	return retVal;
}


inline CLab CLab::operator*(double value) const
{
	CLab retVal = *this;

	retVal *= value;

	return retVal;
}


inline CLab CLab::operator/(double value) const
{
	CLab retVal = *this;

	retVal /= value;

	return retVal;
}


inline CLab& CLab::operator=(const CLab& color)
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


