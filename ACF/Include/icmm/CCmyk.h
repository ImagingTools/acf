#ifndef icmm_CCmyk_included
#define icmm_CCmyk_included


#include "iser/IArchive.h"

#include "icmm/TComposedColor.h"


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
		CI_LAST = CI_BLACK,
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

	bool Serialize(iser::IArchive& archive);

	// operators
	CCmyk operator+(const CCmyk& color) const;
	CCmyk operator-(const CCmyk& color) const;
	CCmyk operator*(const CCmyk& color) const;
	CCmyk operator/(const CCmyk& color) const;

	CCmyk operator*(double value) const;
	CCmyk operator/(double value) const;

	const CCmyk& operator=(const CCmyk& color);

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
	SetComponentValue(CI_CYAN, c);
	SetComponentValue(CI_MAGENTA, m);
	SetComponentValue(CI_YELLOW, y);
	SetComponentValue(CI_BLACK, k);
}


inline CCmyk::CCmyk(const CCmyk& color)
:	BaseClass(color)
{
}


// access to components

inline double CCmyk::GetC() const
{
	return GetComponentValue(CI_CYAN);
}


inline void CCmyk::SetC(double value)
{
	SetComponentValue(CI_CYAN, value);
}


inline double CCmyk::GetM() const
{
	return GetComponentValue(CI_MAGENTA);
}


inline void CCmyk::SetM(double value)
{
	SetComponentValue(CI_MAGENTA, value);
}


inline double CCmyk::GetY() const
{
	return GetComponentValue(CI_YELLOW);
}


inline void CCmyk::SetY(double value)
{
	SetComponentValue(CI_YELLOW, value);
}


inline double CCmyk::GetK() const
{
	return GetComponentValue(CI_BLACK);
}


inline void CCmyk::SetK(double value)
{
	SetComponentValue(CI_BLACK, value);
}


// operators

inline const CCmyk& CCmyk::operator=(const CCmyk& color)
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


#endif // !icmm_CCmyk_included


