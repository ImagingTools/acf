#pragma once


// ACF includes
#include <imath/IDoubleManip.h>


namespace imath
{


/**
	Simple implementation of \c imath::IDoubleManip interface for double values without rounding.
*/
class CDoubleManip: virtual public IDoubleManip
{
public:
	CDoubleManip();

	/**
		Get point character.
	 */
	char GetPointChar() const;

	/**
		Set point character.
	 */
	void SetPointChar(char pointChar);

	// reimplemented (imath::IDoubleManip)
	virtual int GetPrecision() const override;

	// reimplemented (imath::TIValueManip)
	virtual double GetRounded(const double& value) const override;
	virtual bool AreValuesEqual(const double& value1, const double& value2) const override;
	virtual bool IsSmaller(const double& value1, const double& value2) const override;
	virtual bool IsSmallerEqual(const double& value1, const double& value2) const override;
	virtual double GetSmallerValue(const double& value) const override;
	virtual double GetBiggerValue(const double& value) const override;
	virtual QString GetString(const double& value) const override;
	virtual bool GetParsed(const QString& text, double& result) const override;

protected:
	enum{
		PRECISION = 6
	};

private:
	char m_pointChar;
};


// inline methods

inline char CDoubleManip::GetPointChar() const
{
	return m_pointChar;
}


inline void CDoubleManip::SetPointChar(char pointChar)
{
	m_pointChar = pointChar;
}


// reimplemented (imath::TIValueManip)

inline double CDoubleManip::GetRounded(const double& value) const
{
	return value;
}


inline bool CDoubleManip::AreValuesEqual(const double& value1, const double& value2) const
{
	return value1 == value2;
}


inline bool CDoubleManip::IsSmaller(const double& value1, const double& value2) const
{
	return value1 < value2;
}


inline bool CDoubleManip::IsSmallerEqual(const double& value1, const double& value2) const
{
	return value1 <= value2;
}


inline double CDoubleManip::GetSmallerValue(const double& value) const
{
	return value - 0.001;
}


inline double CDoubleManip::GetBiggerValue(const double& value) const
{
	return value + 0.001;
}


} // namespace imath


