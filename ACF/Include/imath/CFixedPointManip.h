#ifndef imath_CRoundedValueManip_included
#define imath_CRoundedValueManip_included


#include "imath/TIValueManip.h"


namespace imath
{


/**
	Implementation of \c imath::IDoubleManip interface for fixed point arithmentic.
*/
class CFixedPointManip: virtual public IDoubleManip
{
public:
	enum RoundingType
	{
		/**
			Normal mathematical rounding to the nearest value.
		*/
		RT_NORMAL,
		/**
			Rounding down.
		*/
		RT_FLOOR,
		/**
			Rounding up.
		*/
		RT_CEIL,
		RT_LAST = RT_CEIL + 1
	};

	CFixedPointManip(int precision = 1, RoundingType roundingType = RT_NORMAL);

	/**
		Get fixed point precision.
		Please note, that also negative values are allowed.
	 */
	int GetPrecision() const;

	/**
		Set fixed point precision.
	 */
	void SetPrecision(int precision);

	/**	Get type of rounding.
	 */
	RoundingType GetRoundingType() const;

	/**	Set type of rounding.
	 */
	void SetRoundingType(RoundingType roundingType);

	/**
		Get point character.
	 */
	char GetPointChar() const;

	/**
		Set point character.
	 */
	void SetPointChar(char pointChar);

	// reimplemented (imath::TIValueManip)
	virtual double GetRounded(const double& value) const;
	virtual bool IsEqual(const double& value1, const double& value2) const;
	virtual bool IsSmaller(const double& value1, const double& value2) const;
	virtual bool IsSmallerEqual(const double& value1, const double& value2) const;
	virtual double GetSmallerValue(const double& value) const;
	virtual double GetBiggerValue(const double& value) const;
	virtual std::string GetString(const double& value) const;
	virtual bool GetParsed(const std::string& text, double& result) const;

protected:
	typedef double (*RoundingFuntionPtr)(double value);

	virtual I_SDWORD GetInternalValue(double value) const;
	virtual double GetNormalValue(I_SDWORD intValue) const;

	std::string GetString(const double& value, int precision) const;
	bool GetParsedUnrounded(const std::string& text, double& result) const;

	static double NormalRoundFuntion(double value);

private:
	int m_precision;
	RoundingType m_roundingType;
	char m_pointChar;

	static RoundingFuntionPtr m_roundingFuntionsPtr[RT_LAST + 1];

	double m_scaleToIntFactor;
};


// inline methods

inline int CFixedPointManip::GetPrecision() const
{
	return m_precision;
}


inline CFixedPointManip::RoundingType CFixedPointManip::GetRoundingType() const
{
	return m_roundingType;
}


inline char CFixedPointManip::GetPointChar() const
{
	return m_pointChar;
}


inline void CFixedPointManip::SetPointChar(char pointChar)
{
	m_pointChar = pointChar;
}


// reimplemented (imath::TIValueManip)

inline double CFixedPointManip::GetRounded(const double& value) const
{
	return GetNormalValue(GetInternalValue(value));
}


inline bool CFixedPointManip::IsEqual(const double& value1, const double& value2) const
{
	return GetInternalValue(value1) == GetInternalValue(value2);
}


inline bool CFixedPointManip::IsSmaller(const double& value1, const double& value2) const
{
	return GetInternalValue(value1) < GetInternalValue(value2);
}


inline bool CFixedPointManip::IsSmallerEqual(const double& value1, const double& value2) const
{
	return GetInternalValue(value1) <= GetInternalValue(value2);
}


inline double CFixedPointManip::GetSmallerValue(const double& value) const
{
	return GetNormalValue(GetInternalValue(value) - 1);
}


inline double CFixedPointManip::GetBiggerValue(const double& value) const
{
	return GetNormalValue(GetInternalValue(value) + 1);
}


// protected methods

inline I_SDWORD CFixedPointManip::GetInternalValue(double value) const
{
	RoundingFuntionPtr function = m_roundingFuntionsPtr[m_roundingType];

	return I_SDWORD(function(value * m_scaleToIntFactor + 0.5));
}


inline double CFixedPointManip::GetNormalValue(I_SDWORD intValue) const
{
	return intValue / m_scaleToIntFactor;
}


} // namespace imath


#endif // !imath_CRoundedValueManip_included


