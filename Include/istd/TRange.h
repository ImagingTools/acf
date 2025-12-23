#pragma once


// STL includes
#include <algorithm>
#include <cmath>

// Qt includes
#include <QtCore/QtGlobal>


namespace istd
{


/**
	Implementation of a abstract range defined by two values - minimum and maximum.
*/
template <typename ValueType>
class TRange
{
public:
	/**
		Constructs an empty range object. IsEmpty() will return \c true for this object.
	*/
	TRange();
	/**
		Copy constructor.
	*/
	TRange(const TRange& range);

	/**
		Constructs a range object from two numbers.
	*/
	TRange(ValueType minValue, ValueType maxValue);

	/**
		Return \c true if the bottom value is smaller or equal then the top value.
	*/
	bool IsValid() const;

	/**
		Return \c true if the bottom value is equal to the top value.
	*/
	bool IsEmpty() const;

	/**
		Return \c true if the range is valid and it is not empty.
	*/
	bool IsValidNonEmpty() const;

	/**
		Get the bottom value.
	*/
	ValueType GetMinValue() const;

	/**
		Get reference to the bottom value.	
	*/
	ValueType& GetMinValueRef();

	/**
		Set the bottom value.
	*/
	void SetMinValue(ValueType minValue);

	/**
		Get the minimum by the absolute value
	*/
	ValueType GetAbsMinValue() const;

	/**
		Get the maximum by the absolute value
	*/
	ValueType GetAbsMaxValue() const;

	/**
		Get the top value.
	*/
	ValueType GetMaxValue() const;

	/**
		Get reference to the top value.
	*/
	ValueType& GetMaxValueRef();

	/**
		Set the top value.
	*/
	void SetMaxValue(ValueType maxValue);

	/**
		Set this range to be empty.
	*/
	void Reset();

	/**
		Get length of this range.
		Length is difference between top and bottom values.
	*/
	ValueType GetLength() const;

	/**
		Get validated range, if it was invalid.
		Range is invalid if set min value is bigger than set max value.
		It returns range with smaller range value as minimum and bigger one as maximum.
	*/
	TRange GetValidated() const;

	/**
		Get validated range, if it was invalid.
		Range is invalid if set min value is bigger than set max value.
		It returns range with smaller range value as minimum and bigger one as maximum.
	*/
	void GetValidated(TRange& result) const;

	/**
		Force this range to be valid.
		Range is invalid if set min value is bigger than set max value.
		It set smaller range value as minimum and bigger one as maximum.
	*/
	void Validate();

	/**
		Returns \c true, if value is in range between \c top and \c bottom.
		This range must be valid.
	*/
	bool Contains(ValueType value) const;

	/**
		Returns \c true, if this range is inside of the \c range.
	*/
	bool Contains(const TRange& range) const;

	/**
		Check if this range is intersected by other range.
		Some range is intersected by another, if intercestion of this two ranges is not empty.
	*/
	bool IsIntersectedBy(const TRange& range) const;

	/**
		Check if this range is outside of the given range.
	*/
	bool IsOutsideOf(const TRange& range) const;

	/**
		Get translated position range.
		Translation adds the offset value to minimal and mavimal values.
	*/
	TRange GetTranslated(ValueType offset) const;

	/**
		Translated position of this range.
		Translation adds the offset value to minimal and mavimal values.
	*/
	void Translate(ValueType offset);

	/**
		Get intersection with the second \c range.
	*/
	TRange GetIntersection(const TRange& range) const;

	/**
		Set this range to be intersection of two ranges.
	*/
	void Intersection(const TRange& range);

	/**
		Get union with the second \c range.
	*/
	TRange GetUnion(const TRange& range) const;

	/**
		Get union with the second \c range.
	*/
	TRange GetUnion(double value) const;

	/**
		Set this range to be union of two ranges.
	*/
	void Unite(const TRange& range);

	/**
		Set this range to be union of two ranges.
	*/
	void Unite(double value);

	/**
		Get expanded range using the second \c range.
		Expanding operation calculate simple sum for each components separately.
	*/
	TRange GetExpanded(const TRange& range) const;

	/**
		Set this range to be expanded.
		Expanding operation calculate simple sum for each components separately.
	*/
	void Expand(const TRange& range);

	/**
		Get distance from value to range.
		Distance is the smallest absolute value difference of given value and the values belonging to range.
	*/
	ValueType GetDistance(ValueType value) const;
	/**
		Get nearest value belonging to range.
	*/
	ValueType GetNearestInside(ValueType value) const;

	/**
		Get value clipped to the range.
	*/
	ValueType GetClipped(ValueType value) const;

	/**
		Get range clipped to the current range.
	*/
	TRange GetClipped(const TRange& range) const;

	/**
		Get value based on 'alpha' factor.
		\param	alpha	alpha factor. If it is 0, minimum range value will be returned.
						If it is 1, maximum range value will be returned.
						Rest values are linear interpolated.
	*/
	ValueType GetValueFromAlpha(double alpha) const;

	/**
		Get value based on 'alpha' factor.
		\param	alpha	alpha factor. If it is 0, minimum range value will be returned.
						If it is 1, maximum range value will be returned.
						Rest values are linear interpolated.
	*/
	double GetAlphaFromValue(ValueType value) const;

	/**
		Returns the value, that corresponds the input value \c inputValue in the other range \c range.
							It is the same as \c range.GetValueFromAlpha(GetAlphaFromValue(value)).
		\param	value		value definde in this range.
							If it is equal to minimum value in this range,
							it will be mapped to minimum value of \c range.
							If it is equal to maximum value in this range,
							it will be mapped to maximum value of \c range.
							Rest values are linear interpolated.
							\note This value must be in current range.
	*/
	ValueType GetMappedTo(ValueType value, const TRange& range) const;

	/** 
		Get a combined range.
		It calculates combined range. Range [0, 1] is neutral element of this operation.
	*/
	TRange GetApply(const TRange& range) const;
	
	TRange GetInvertApply(const TRange& range) const;

	TRange GetInverted() const;

	/**
		Set this range to be linear interpolated using two other ranges and adjast alpha value.
	*/
	void SetInterpolated(const TRange& first, const TRange& second, double alpha);

	TRange& operator=(const TRange& range);
	bool operator==(const TRange& range) const;
	bool operator!=(const TRange& range) const;
	TRange operator*(double value) const;
	TRange operator/(double value) const;
	TRange& operator*=(double value);
	TRange& operator/=(double value);

	// static methods
	/**
		Return null range.
	*/
	static const TRange& GetNull();
	/**
		Return invalid range.
	*/
	static const TRange& GetInvalid();
	/**
		Return always valid range.
	*/
	static TRange GetValid(ValueType value1, ValueType value2);

private:
	ValueType m_minValue;
	ValueType m_maxValue;
};


// public methods

template <typename ValueType>
TRange<ValueType>::TRange()
	:m_minValue(0),
	m_maxValue(0)
{
}


template <typename ValueType>
TRange<ValueType>::TRange(const TRange& range)
	:m_minValue(range.m_minValue),
	m_maxValue(range.m_maxValue)
{
}


template <typename ValueType>
TRange<ValueType>::TRange(ValueType minValue, ValueType maxValue)
	:m_minValue(minValue),
	m_maxValue(maxValue)
{
}


template <typename ValueType>
inline bool TRange<ValueType>::IsValid() const
{
	return (m_minValue <= m_maxValue);
}


template <typename ValueType>
inline bool TRange<ValueType>::IsEmpty() const
{
	return (m_minValue == m_maxValue);
}


template <typename ValueType>
inline bool TRange<ValueType>::IsValidNonEmpty() const
{
	return (m_minValue < m_maxValue);
}


template <typename ValueType>
inline ValueType TRange<ValueType>::GetMinValue() const
{
	return m_minValue;
}


template <typename ValueType>
inline ValueType& TRange<ValueType>::GetMinValueRef()
{
	return m_minValue;
}


template <typename ValueType>
inline void TRange<ValueType>::SetMinValue(ValueType minValue)
{
	m_minValue = minValue;
}


template <typename ValueType>
ValueType TRange<ValueType>::GetAbsMinValue() const
{
	ValueType absMin = std::abs(m_minValue);
	ValueType absMax = std::abs(m_maxValue);

	return std::min<ValueType>(absMin , absMax);
}


template <typename ValueType>
ValueType TRange<ValueType>::GetAbsMaxValue() const
{
	ValueType absMin = std::abs(m_minValue);
	ValueType absMax = std::abs(m_maxValue);

	return std::max<ValueType>(absMin, absMax);
}


template <typename ValueType>
inline ValueType TRange<ValueType>::GetMaxValue() const
{
	return m_maxValue;
}


template <typename ValueType>
inline ValueType& TRange<ValueType>::GetMaxValueRef()
{
	return m_maxValue;
}


template <typename ValueType>
inline void TRange<ValueType>::SetMaxValue(ValueType maxValue)
{
	m_maxValue = maxValue;
}


template <typename ValueType>
void TRange<ValueType>::Reset()
{
	m_minValue = ValueType(0);
	m_maxValue = ValueType(0);
}


template <typename ValueType>
inline ValueType TRange<ValueType>::GetLength() const
{
	return IsValid() ? m_maxValue - m_minValue: 0;
}


template <typename ValueType>
inline TRange<ValueType> TRange<ValueType>::GetValidated() const
{
	return TRange(
				std::min<ValueType>(m_minValue, m_maxValue),
				std::max<ValueType>(m_minValue, m_maxValue));
}


template <typename ValueType>
inline void TRange<ValueType>::GetValidated(TRange& result) const
{
	result.SetMinValue(std::min<ValueType>(m_minValue, m_maxValue));
	result.SetMaxValue(std::max<ValueType>(m_minValue, m_maxValue));
}


template <typename ValueType>
inline void TRange<ValueType>::Validate()
{
	*this = TRange(std::min<ValueType>(m_minValue, m_maxValue), std::max<ValueType>(m_minValue, m_maxValue));
}


template <typename ValueType>
inline bool TRange<ValueType>::Contains(ValueType value) const
{
	Q_ASSERT(IsValid());

	return (value  >= m_minValue) && (value <= m_maxValue);
}


template <typename ValueType>
inline bool TRange<ValueType>::Contains(const TRange& range) const
{
	Q_ASSERT(IsValid());
	Q_ASSERT(range.IsValid());

	return (range.m_minValue >= m_minValue) && (range.m_maxValue <= m_maxValue);
}


template <typename ValueType>
inline bool TRange<ValueType>::IsIntersectedBy(const TRange& range) const
{
	return (range.m_maxValue > m_minValue) && (range.m_minValue < m_maxValue);
}


template <typename ValueType>
inline bool TRange<ValueType>::IsOutsideOf(const TRange& range) const
{
	return (range.m_maxValue <= m_minValue) || (range.m_minValue >= m_maxValue);
}


template <typename ValueType>
inline TRange<ValueType> TRange<ValueType>::GetTranslated(ValueType offset) const
{
	return TRange(m_minValue + offset, m_maxValue + offset);
}


template <typename ValueType>
inline void TRange<ValueType>::Translate(ValueType offset)
{
	m_minValue += offset;
	m_maxValue += offset;
}


template <typename ValueType>
inline TRange<ValueType> TRange<ValueType>::GetIntersection(const TRange& range) const
{
	if (IsEmpty()){
		return *this;
	}
	else{
		return TRange(std::max<ValueType>(m_minValue, range.m_minValue), std::min<ValueType>(m_maxValue, range.m_maxValue));
	}
}


template <typename ValueType>
void TRange<ValueType>::Intersection(const TRange& range)
{
	if (!IsEmpty()){
		if (range.m_minValue > m_minValue){
			m_minValue = range.m_minValue;
		}

		if (range.m_maxValue < m_maxValue){
			m_maxValue = range.m_maxValue;
		}
	}
}


template <typename ValueType>
TRange<ValueType> TRange<ValueType>::GetUnion(const TRange& range) const
{
	if (range.IsValid()){
		if (IsValid()){
			return TRange(std::min<ValueType>(m_minValue, range.m_minValue), std::max<ValueType>(m_maxValue, range.m_maxValue));
		}
		else{
			return range;
		}
	}
	else{
		return *this;
	}
}


template <typename ValueType>
TRange<ValueType> TRange<ValueType>::GetUnion(double value) const
{
	if (IsValid()){
		return TRange(std::min<ValueType>(m_minValue, value), std::max<ValueType>(m_maxValue, value));
	}
	else{
		return TRange(value, value);
	}
}


template <typename ValueType>
void TRange<ValueType>::Unite(const TRange& range)
{
	if (range.IsValid()){
		if (IsValid()){
			if (range.m_minValue < m_minValue){
				m_minValue = range.m_minValue;
			}

			if (range.m_maxValue > m_maxValue){
				m_maxValue = range.m_maxValue;
			}
		}
		else{
			*this = range;
		}
	}
}


template <typename ValueType>
void TRange<ValueType>::Unite(double value)
{
	if (IsValid()){
		if (value < m_minValue){
			m_minValue = value;
		}

		if (value > m_maxValue){
			m_maxValue = value;
		}
	}
	else{
		m_minValue = value;
		m_maxValue = value;
	}
}


template <typename ValueType>
inline TRange<ValueType> TRange<ValueType>::GetExpanded(const TRange& range) const
{
	return TRange(m_minValue + range.m_minValue, m_maxValue + range.m_maxValue);
}


template <typename ValueType>
inline void TRange<ValueType>::Expand(const TRange& range)
{
	m_minValue += range.m_minValue;
	m_maxValue += range.m_maxValue;
}


template <typename ValueType>
inline ValueType TRange<ValueType>::GetDistance(ValueType value) const
{
	if (value > m_maxValue){
		return value - m_maxValue;
	}

	if (value < m_minValue){
		return m_minValue - value;
	}

	return 0;
}


template <typename ValueType>
inline ValueType TRange<ValueType>::GetNearestInside(ValueType value) const
{
	if (value > m_maxValue){
		value = m_maxValue;
	}

	if (value < m_minValue){
		value = m_minValue;
	}

	return value;
}


template <typename ValueType>
ValueType TRange<ValueType>::GetClipped(ValueType value) const
{
	if (!Contains(value)){
		ValueType distanceMin = (ValueType)qAbs(value - m_minValue);
		ValueType distanceMax = (ValueType)qAbs(value - m_maxValue);

		return distanceMin < distanceMax ? m_minValue : m_maxValue;
	}

	return value;
}

	
template <typename ValueType>
TRange<ValueType> TRange<ValueType>::GetClipped(const TRange& range) const
{
	istd::TRange<ValueType> clippedRange = range;

	if (!Contains(range.GetMinValue())){
		clippedRange.SetMinValue(m_minValue);
	}
	
	if (!Contains(range.GetMaxValue())){
		clippedRange.SetMaxValue(m_maxValue);
	}

	return clippedRange;
}


template <typename ValueType>
inline ValueType TRange<ValueType>::GetValueFromAlpha(double alpha) const
{
	return GetMinValue() + alpha * GetLength();
}


template <typename ValueType>
inline double TRange<ValueType>::GetAlphaFromValue(ValueType value) const
{
	auto length = static_cast<double>(GetLength());
	if (length)
		return static_cast<double>(value - GetMinValue()) / length;
	else
		return 0.0;
}


template <typename ValueType>
inline ValueType TRange<ValueType>::GetMappedTo(ValueType value, const TRange& range) const
{
	ValueType length = GetLength();
	if (length)
		return ValueType(range.GetMinValue() + (value - GetMinValue()) * (range.GetLength() / length));
	else
		return (ValueType) 0.0;
}


template <typename ValueType>
TRange<ValueType> TRange<ValueType>::GetApply(const TRange& range) const
{
	TRange result;
	result.m_minValue = GetValueFromAlpha(range.m_minValue);
	result.m_maxValue = GetValueFromAlpha(range.m_maxValue);

	return result;
}


template <typename ValueType>
TRange<ValueType> TRange<ValueType>::GetInvertApply(const TRange& range) const
{
	TRange result;

	double length = GetLength();
	if (length)
	{
		result.m_minValue = ValueType((range.m_minValue - m_minValue) / length);
		result.m_maxValue = ValueType((range.m_maxValue - m_minValue) / length);
	}
	else
	{
		result.m_minValue = result.m_maxValue = 0.0;
	}

	return result;
}


template <typename ValueType>
TRange<ValueType> TRange<ValueType>::GetInverted() const
{
	TRange result;

	ValueType minusWidth = m_minValue - m_maxValue;
	if (minusWidth)
	{
		result.m_minValue = ValueType(m_minValue / (double)minusWidth);
		result.m_maxValue = ValueType((m_minValue - 1) / (double)minusWidth);
	}
	else
	{
		result.m_minValue = result.m_maxValue = 0.0;
	}

	return result;
}


template <typename ValueType>
void TRange<ValueType>::SetInterpolated(const TRange& first, const TRange& second, double alpha)
{
	double beta = 1 - alpha;

	m_minValue = ValueType(first.m_minValue * beta + second.m_minValue * alpha);
	m_maxValue = ValueType(first.m_maxValue * beta + second.m_maxValue * alpha);
}


template <typename ValueType>
inline bool TRange<ValueType>::operator==(const TRange& range) const
{
	return (m_maxValue == range.m_maxValue) && (m_minValue == range.m_minValue);
}


template <typename ValueType>
inline bool TRange<ValueType>::operator!=(const TRange& range) const
{
	return (m_maxValue != range.m_maxValue) || (m_minValue != range.m_minValue);
}



template <typename ValueType>
inline TRange<ValueType>& TRange<ValueType>::operator=(const TRange& range)
{
	m_maxValue = range.m_maxValue;
	m_minValue = range.m_minValue;

	return *this;
}


template <typename ValueType>
inline TRange<ValueType> TRange<ValueType>::operator*(double value) const
{
	TRange result = *this;

	result.m_minValue *= value;
	result.m_maxValue *= value;

	return result;
}


template <typename ValueType>
inline TRange<ValueType> TRange<ValueType>::operator/(double value) const
{
	TRange result = *this;

	if (value)
	{
		result.m_minValue /= value;
		result.m_maxValue /= value;
	}
	else
	{
		result.m_minValue = result.m_maxValue = 0.0;
	}

	return result;
}


template <typename ValueType>
inline TRange<ValueType>& TRange<ValueType>::operator*=(double value)
{
	m_minValue *= value;
	m_maxValue *= value;

	return *this;
}


template <typename ValueType>
inline TRange<ValueType>& TRange<ValueType>::operator/=(double value)
{
	if (value)
	{
		m_minValue /= value;
		m_maxValue /= value;
	}
	else
	{
		m_minValue = m_maxValue = 0.0;
	}

	return *this;
}


// static methods

template <typename ValueType>
inline TRange<ValueType> TRange<ValueType>::GetValid(ValueType value1, ValueType value2)
{
	return TRange(std::min<ValueType>(value1, value2), std::max<ValueType>(value1, value2));
}


// static methods

template <typename ValueType>
inline const TRange<ValueType>& TRange<ValueType>::GetNull()
{
	static const TRange<ValueType> s_null(0, 0);
	return s_null;
}


template <typename ValueType>
inline const TRange<ValueType>& TRange<ValueType>::GetInvalid()
{
	static const TRange<ValueType> s_invalid(0, -1);
	return s_invalid;
}


// typedefs

typedef istd::TRange<double> CRange;
typedef istd::TRange<int> CIntRange;


} // namespace istd


