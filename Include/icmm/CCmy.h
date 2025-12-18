#pragma once


// ACF includes
#include <iser/IArchive.h>
#include <icmm/TComposedColor.h>


namespace icmm
{


/**
	Primitive for representation of CMY color value.
	Please note, that normal components values are in range [0, 1].
*/
class CCmy: public TComposedColor<3>
{
public:
	typedef TComposedColor<3> BaseClass;

	enum ComponentIndex
	{
		CI_CYAN,
		CI_MAGENTA,
		CI_YELLOW,
		CI_LAST = CI_YELLOW
	};

	CCmy(double c = 0.0, double m = 0.0, double y = 0.0);
	CCmy(const CCmy& color);

	// access to component values
	double GetC() const;
	void SetC(double value);
	double GetM() const;
	void SetM(double value);
	double GetY() const;
	void SetY(double value);

	virtual bool Serialize(iser::IArchive& archive) override;

	// operators
	CCmy operator+(const CCmy& color) const;
	CCmy operator-(const CCmy& color) const;
	CCmy operator*(const CCmy& color) const;
	CCmy operator/(const CCmy& color) const;

	CCmy operator*(double value) const;
	CCmy operator/(double value) const;

	CCmy& operator=(const CCmy& color);

	const CCmy& operator+=(const CCmy& color);
	const CCmy& operator-=(const CCmy& color);
	const CCmy& operator*=(const CCmy& color);
	const CCmy& operator/=(const CCmy& color);

	const CCmy& operator*=(double value);
	const CCmy& operator/=(double value);
};


// inline methods

inline CCmy::CCmy(double c, double m, double y)
{
	SetElement(CI_CYAN, c);
	SetElement(CI_MAGENTA, m);
	SetElement(CI_YELLOW, y);
}


inline CCmy::CCmy(const CCmy& color)
:	BaseClass(color)
{
}


// access to components

inline double CCmy::GetC() const
{
	return GetElement(CI_CYAN);
}


inline void CCmy::SetC(double value)
{
	SetElement(CI_CYAN, value);
}


inline double CCmy::GetM() const
{
	return GetElement(CI_MAGENTA);
}


inline void CCmy::SetM(double value)
{
	SetElement(CI_MAGENTA, value);
}


inline double CCmy::GetY() const
{
	return GetElement(CI_YELLOW);
}


inline void CCmy::SetY(double value)
{
	SetElement(CI_YELLOW, value);
}


// operators

inline CCmy& CCmy::operator=(const CCmy& color)
{
	BaseClass::operator=(color);

	return *this;
}


inline const CCmy& CCmy::operator+=(const CCmy& color)
{
	BaseClass::operator+=(color);

	return *this;
}


inline const CCmy& CCmy::operator-=(const CCmy& color)
{
	BaseClass::operator-=(color);

	return *this;
}


inline const CCmy& CCmy::operator*=(const CCmy& color)
{
	BaseClass::operator*=(color);

	return *this;
}


inline const CCmy& CCmy::operator/=(const CCmy& color)
{
	BaseClass::operator/=(color);

	return *this;
}


inline const CCmy& CCmy::operator*=(double value)
{
	BaseClass::operator*=(value);

	return *this;
}


inline const CCmy& CCmy::operator/=(double value)
{
	BaseClass::operator/=(value);

	return *this;
}


} // namespace icmm


