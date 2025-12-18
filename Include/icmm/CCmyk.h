#pragma once


// ACF includes
#include <iser/IArchive.h>
#include <icmm/TComposedColor.h>


namespace icmm
{


/**
	Primitive for representation of CMYK color value.
	Please note, that normal components values are in range [0, 1].
*/
class CCmyk: public TComposedColor<4>
{
public:
	typedef TComposedColor<4> BaseClass;

	enum ComponentIndex
	{
		CI_CYAN,
		CI_MAGENTA,
		CI_YELLOW,
		CI_BLACK,
		CI_LAST = CI_BLACK
	};

	CCmyk(double c = 0.0, double m = 0.0, double y = 0.0, double k = 0.0);
	CCmyk(const CCmyk& color);

	// access to component values
	double GetC() const;
	void SetC(double value);
	double GetM() const;
	void SetM(double value);
	double GetY() const;
	void SetY(double value);
	double GetK() const;
	void SetK(double value);

	virtual bool Serialize(iser::IArchive& archive) override;

	// operators
	CCmyk operator+(const CCmyk& color) const;
	CCmyk operator-(const CCmyk& color) const;
	CCmyk operator*(const CCmyk& color) const;
	CCmyk operator/(const CCmyk& color) const;

	CCmyk operator*(double value) const;
	CCmyk operator/(double value) const;

	CCmyk& operator=(const CCmyk& color);

	const CCmyk& operator+=(const CCmyk& color);
	const CCmyk& operator-=(const CCmyk& color);
	const CCmyk& operator*=(const CCmyk& color);
	const CCmyk& operator/=(const CCmyk& color);

	const CCmyk& operator*=(double value);
	const CCmyk& operator/=(double value);
};


// inline methods

inline CCmyk::CCmyk(double c, double m, double y, double k)
{
	SetElement(CI_CYAN, c);
	SetElement(CI_MAGENTA, m);
	SetElement(CI_YELLOW, y);
	SetElement(CI_BLACK, k);
}


inline CCmyk::CCmyk(const CCmyk& color)
:	BaseClass(color)
{
}


// access to components

inline double CCmyk::GetC() const
{
	return GetElement(CI_CYAN);
}


inline void CCmyk::SetC(double value)
{
	SetElement(CI_CYAN, value);
}


inline double CCmyk::GetM() const
{
	return GetElement(CI_MAGENTA);
}


inline void CCmyk::SetM(double value)
{
	SetElement(CI_MAGENTA, value);
}


inline double CCmyk::GetY() const
{
	return GetElement(CI_YELLOW);
}


inline void CCmyk::SetY(double value)
{
	SetElement(CI_YELLOW, value);
}


inline double CCmyk::GetK() const
{
	return GetElement(CI_BLACK);
}


inline void CCmyk::SetK(double value)
{
	SetElement(CI_BLACK, value);
}


// operators

inline CCmyk& CCmyk::operator=(const CCmyk& color)
{
	BaseClass::operator=(color);

	return *this;
}


inline const CCmyk& CCmyk::operator+=(const CCmyk& color)
{
	BaseClass::operator+=(color);

	return *this;
}


inline const CCmyk& CCmyk::operator-=(const CCmyk& color)
{
	BaseClass::operator-=(color);

	return *this;
}


inline const CCmyk& CCmyk::operator*=(const CCmyk& color)
{
	BaseClass::operator*=(color);

	return *this;
}


inline const CCmyk& CCmyk::operator/=(const CCmyk& color)
{
	BaseClass::operator/=(color);

	return *this;
}


inline const CCmyk& CCmyk::operator*=(double value)
{
	BaseClass::operator*=(value);

	return *this;
}


inline const CCmyk& CCmyk::operator/=(double value)
{
	BaseClass::operator/=(value);

	return *this;
}


} // namespace icmm


