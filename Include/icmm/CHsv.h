#pragma once


// ACF includes
#include <iser/IArchive.h>
#include <icmm/TComposedColor.h>


namespace icmm
{


/**
	Primitive for representation of HSV color value.
	Please note, that normal components values are in range [0, 1].
*/
class CHsv: public TComposedColor<3>
{
public:
	typedef TComposedColor<3> BaseClass;

	enum ComponentIndex
	{
		CI_HUE,
		CI_SATURATION,
		CI_VALUE,
		CI_LAST = CI_VALUE
	};

	CHsv(double h = 0.0, double s = 0.0, double v = 0.0);
	CHsv(const CHsv& color);
	CHsv(const imath::CVarVector& vector);

	// access to component values
	double GetHue() const;
	void SetHue(double value);
	double GetSaturation() const;
	void SetSaturation(double value);
	double GetValue() const;
	void SetValue(double value);

	virtual bool Serialize(iser::IArchive& archive) override;

	// operators
	CHsv operator+(const CHsv& color) const;
	CHsv operator-(const CHsv& color) const;
	CHsv operator*(const CHsv& color) const;
	CHsv operator/(const CHsv& color) const;

	CHsv operator*(double value) const;
	CHsv operator/(double value) const;

	CHsv& operator=(const CHsv& color);

	const CHsv& operator+=(const CHsv& color);
	const CHsv& operator-=(const CHsv& color);
	const CHsv& operator*=(const CHsv& color);
	const CHsv& operator/=(const CHsv& color);

	const CHsv& operator*=(double value);
	const CHsv& operator/=(double value);
};


// inline methods

inline CHsv::CHsv(double h, double s, double v)
{
	SetElement(CI_HUE, h);
	SetElement(CI_SATURATION, s);
	SetElement(CI_VALUE, v);
}


inline CHsv::CHsv(const CHsv& color)
:	BaseClass(color)
{
}


inline CHsv::CHsv(const imath::CVarVector& vector)
:	BaseClass(vector)
{
}


// access to components

inline double CHsv::GetHue() const
{
	return GetElement(CI_HUE);
}


inline void CHsv::SetHue(double value)
{
	SetElement(CI_HUE, value);
}


inline double CHsv::GetSaturation() const
{
	return GetElement(CI_SATURATION);
}


inline void CHsv::SetSaturation(double value)
{
	SetElement(CI_SATURATION, value);
}


inline double CHsv::GetValue() const
{
	return GetElement(CI_VALUE);
}


inline void CHsv::SetValue(double value)
{
	SetElement(CI_VALUE, value);
}



// operators

inline CHsv& CHsv::operator=(const CHsv& color)
{
	BaseClass::operator=(color);

	return *this;
}


inline const CHsv& CHsv::operator+=(const CHsv& color)
{
	BaseClass::operator+=(color);

	return *this;
}


inline const CHsv& CHsv::operator-=(const CHsv& color)
{
	BaseClass::operator-=(color);

	return *this;
}


inline const CHsv& CHsv::operator*=(const CHsv& color)
{
	BaseClass::operator*=(color);

	return *this;
}


inline const CHsv& CHsv::operator/=(const CHsv& color)
{
	BaseClass::operator/=(color);

	return *this;
}


inline const CHsv& CHsv::operator*=(double value)
{
	BaseClass::operator*=(value);

	return *this;
}


inline const CHsv& CHsv::operator/=(double value)
{
	BaseClass::operator/=(value);

	return *this;
}


} // namespace icmm


