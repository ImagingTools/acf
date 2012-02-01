#ifndef imath_TWeightedNumber_included
#define imath_TWeightedNumber_included


#include "istd/istd.h"


namespace imath
{


/**
	This class implements number with a weight and related arithmetic operations.
*/
template <class ValueT, class WeightT = ValueT>
class TWeightedNumber
{
public:
	typedef ValueT ValueType;
	typedef WeightT WeightType;

	TWeightedNumber();
	TWeightedNumber(const TWeightedNumber<ValueT, WeightT>& value);
	explicit TWeightedNumber(const ValueType& sum, const WeightType& weight = 1);

	/**
		Set this value to unimportant state (weight to 0)
	*/
	void Reset();

	/**
		Checks if this value is importantant.
		It is true, of weight is bigger than imath_EPSYLON.
	*/
	bool IsImportant() const;

	/**
		Get a sum part of weighted number.
		You should to get weight part too, to calculate real value.
	*/
	const ValueType& GetSum() const;
	void SetSum(const ValueType& sum);
	/**
		Get a weight part of this number.
	*/
	const WeightType& GetWeight() const;
	void SetWeight(const WeightType& weight);
	/**
		Get a real value of this number.
	*/
	ValueType GetValue() const;

	/**
		Set new weight, without change represented value.
	*/
	void Normalize(const WeightType& weight = 1);
	TWeightedNumber<ValueT, WeightT> GetNormalized(const WeightType& weight = 1) const;
	void GetNormalized(const WeightType& weight, TWeightedNumber<ValueT, WeightT>& result) const;

	void Scale(double scale);
	TWeightedNumber<ValueT, WeightT> GetScaled(double scale) const;
	void GetScaled(double scale, TWeightedNumber<ValueT, WeightT>& result) const;

	/**
		Add a value to sum part.
	*/
	void AddSum(ValueType value);
	/**
		Add a value to weight part.
	*/
	void AddWeight(WeightType weight);
	/**
		Subtract a value to sum part.
	*/
	void SubSum(ValueType value);
	/**
		Subtract a value to weight part.
	*/
	void SubWeight(WeightType weight);
	/**
		Simple add operation.
		Difference between operator+ and this method is,
		that operator+ return a value with weight which sum of weights is, but this method
		is faster and returns value with undefined weight, but correct real value.
		It means, when you are interested only in real value, than you can use this method.
	*/
	void SimpleAdd(const TWeightedNumber<ValueT, WeightT>& value);
	/**
		Get simple sum.
		Please read notes to GetSimpleAdd.
	*/
	TWeightedNumber<ValueT, WeightT> GetSimpleAdd(const TWeightedNumber<ValueT, WeightT>& value) const;
	/**
		Get simple sum.
		Please read notes to GetSimpleAdd.
		\overload
	*/
	void GetSimpleAdd(const TWeightedNumber<ValueT, WeightT>& value, TWeightedNumber<ValueT, WeightT>& result) const;
	/**
		Simple subtraction operation.
		Difference between operator- and this method is,
		that operator- return a value with weight which sum of weights is, but this
		is faster and returns value with undefined weight, but correct real value.
		It means, when you are interested only in real value, than you can use this method.
	*/
	void SimpleSub(const TWeightedNumber<ValueT, WeightT>& value);
	/**
		Get simple sum.
		Please read notes to GetSimpleAdd.
	*/
	TWeightedNumber<ValueT, WeightT> GetSimpleSub(const TWeightedNumber<ValueT, WeightT>& value) const;
	/**
		Get simple sum.
		Please read notes to GetSimpleAdd.
		\overload
	*/
	void GetSimpleSub(const TWeightedNumber<ValueT, WeightT>& value, TWeightedNumber<ValueT, WeightT>& result) const;

	/**
		Do addition.
		Please note, that operator+= calculte only simple sum of coefficients.
	*/
	void Add(const TWeightedNumber<ValueT, WeightT>& value);
	/**
		Get result of addition.
		Please note, that operator+ calculte only simple sum of coefficients.
	*/
	TWeightedNumber<ValueT, WeightT> GetAdd(const TWeightedNumber<ValueT, WeightT>& value) const;
	/**
		Get result of addition.
		Please note, that operator+ calculte only simple sum of coefficients.
	*/
	void GetAdd(const TWeightedNumber<ValueT, WeightT>& value, TWeightedNumber<ValueT, WeightT>& result) const;
	/**
		Do Subtraction.
		Please note, that operator-= calculte only simple difference of coefficients.
	*/
	void Sub(const TWeightedNumber<ValueT, WeightT>& value);
	/**
		Get result of addition.
		Please note, that operator- calculte only simple sum of coefficients.
	*/
	TWeightedNumber<ValueT, WeightT> GetSub(const TWeightedNumber<ValueT, WeightT>& value) const;
	/**
		Get result of addition.
		Please note, that operator- calculte only simple sum of coefficients.
	*/
	void GetSub(const TWeightedNumber<ValueT, WeightT>& value, TWeightedNumber<ValueT, WeightT>& result) const;

	/**
		Calculate simple sum of coefficients.
	*/
	TWeightedNumber<ValueT, WeightT> operator+(const TWeightedNumber<ValueT, WeightT>& value) const;
	/**
		Calculate simple difference of coefficients.
	*/
	TWeightedNumber<ValueT, WeightT> operator-(const TWeightedNumber<ValueT, WeightT>& value) const;
	/**
		Calculate simple multiplication of coefficients.
	*/
	TWeightedNumber<ValueT, WeightT> operator*(const TWeightedNumber<ValueT, WeightT>& value) const;
	/**
		Calculate simple divide of coefficients.
	*/
	TWeightedNumber<ValueT, WeightT> operator/(const TWeightedNumber<ValueT, WeightT>& value) const;

	const TWeightedNumber<ValueT, WeightT>& operator=(const TWeightedNumber<ValueT, WeightT>& value);
	/**
		Calculate simple sum of coefficients.
	*/
	const TWeightedNumber<ValueT, WeightT>& operator+=(const TWeightedNumber<ValueT, WeightT>& value);
	/**
		Calculate simple difference of coefficients.
	*/
	const TWeightedNumber<ValueT, WeightT>& operator-=(const TWeightedNumber<ValueT, WeightT>& value);
	/**
		Calculate simple multiplication of coefficients.
	*/
	const TWeightedNumber<ValueT, WeightT>& operator*=(const TWeightedNumber<ValueT, WeightT>& value);
	/**
		Calculate simple divide of coefficients.
	*/
	const TWeightedNumber<ValueT, WeightT>& operator/=(const TWeightedNumber<ValueT, WeightT>& value);

	bool operator==(const TWeightedNumber<ValueT, WeightT>& number) const;
	bool operator!=(const TWeightedNumber<ValueT, WeightT>& number) const;

	//	arithmetic operators with scalar
	TWeightedNumber<ValueT, WeightT> operator+(ValueType scalar) const;
	TWeightedNumber<ValueT, WeightT> operator-(ValueType scalar) const;
	TWeightedNumber<ValueT, WeightT> operator*(double scalar) const;
	TWeightedNumber<ValueT, WeightT> operator/(double scalar) const;

private:
	ValueType m_sum;
	WeightType m_weight;
};


// public methods

template <class ValueT, class WeightT>
TWeightedNumber<ValueT, WeightT> TWeightedNumber<ValueT, WeightT>::GetNormalized(const WeightType& weight) const
{
	TWeightedNumber<ValueT, WeightT> result;

	GetNormalized(weight, result);

	return result;
}


template <class ValueT, class WeightT>
TWeightedNumber<ValueT, WeightT> TWeightedNumber<ValueT, WeightT>::GetScaled(double scale) const
{
	TWeightedNumber<ValueT, WeightT> result;

	GetScaled(scale, result);

	return result;
}


// inline methods

template <class ValueT, class WeightT>
inline TWeightedNumber<ValueT, WeightT>::TWeightedNumber()
{
}


template <class ValueT, class WeightT>
inline TWeightedNumber<ValueT, WeightT>::TWeightedNumber(const TWeightedNumber<ValueT, WeightT>& value)
{
	*this = value;
}


template <class ValueT, class WeightT>
inline TWeightedNumber<ValueT, WeightT>::TWeightedNumber(const ValueType& sum, const WeightT& weight)
{
	m_sum = sum;
	m_weight = weight;
}


template <class ValueT, class WeightT>
void TWeightedNumber<ValueT, WeightT>::Reset()
{
	m_sum = 0;
	m_weight = 0;
}


template <class ValueT, class WeightT>
inline bool TWeightedNumber<ValueT, WeightT>::IsImportant() const
{
	return m_weight > I_EPSILON;
}


template <class ValueT, class WeightT>
const ValueT& TWeightedNumber<ValueT, WeightT>::GetSum() const
{
	return m_sum;
}


template <class ValueT, class WeightT>
void TWeightedNumber<ValueT, WeightT>::SetSum(const ValueType& sum)
{
	m_sum = sum;
}


template <class ValueT, class WeightT>
const typename TWeightedNumber<ValueT, WeightT>::WeightType& TWeightedNumber<ValueT, WeightT>::GetWeight() const
{
	return m_weight;
}


template <class ValueT, class WeightT>
void TWeightedNumber<ValueT, WeightT>::SetWeight(const WeightType& weight)
{
	m_weight = weight;
}


template <class ValueT, class WeightT>
typename TWeightedNumber<ValueT, WeightT>::ValueType TWeightedNumber<ValueT, WeightT>::GetValue() const
{
	I_ASSERT(IsImportant());

	return (m_sum / m_weight);
}


template <class ValueT, class WeightT>
inline void TWeightedNumber<ValueT, WeightT>::Normalize(const WeightType& weight)
{
	TWeightedNumber<ValueT, WeightT>(m_sum * weight / m_weight, weight);
}


template <class ValueT, class WeightT>
inline void TWeightedNumber<ValueT, WeightT>::GetNormalized(const WeightType& weight, TWeightedNumber<ValueT, WeightT>& result) const
{
	result.m_sum = m_sum * weight / m_weight;
	result.m_weight = weight;
}


template <class ValueT, class WeightT>
inline void TWeightedNumber<ValueT, WeightT>::Scale(double scale)
{
	m_sum *= scale;
	m_weight *= scale;
}


template <class ValueT, class WeightT>
inline void TWeightedNumber<ValueT, WeightT>::GetScaled(double scale, TWeightedNumber<ValueT, WeightT>& result) const
{
	result.m_sum = m_sum * scale;
	result.m_weight = m_weight * scale;
}


template <class ValueT, class WeightT>
inline void TWeightedNumber<ValueT, WeightT>::AddSum(ValueType value)
{
	m_sum += value;
}


template <class ValueT, class WeightT>
inline void TWeightedNumber<ValueT, WeightT>::AddWeight(WeightType weight)
{
	m_weight += weight;
}


template <class ValueT, class WeightT>
inline void TWeightedNumber<ValueT, WeightT>::SubSum(ValueType value)
{
	m_sum -= value;
}


template <class ValueT, class WeightT>
inline void TWeightedNumber<ValueT, WeightT>::SubWeight(WeightType weight)
{
	m_weight -= weight;
}


template <class ValueT, class WeightT>
inline void TWeightedNumber<ValueT, WeightT>::SimpleAdd(const TWeightedNumber<ValueT, WeightT>& value)
{
	m_sum = (m_sum * value.m_weight + value.m_sum * m_weight);
	m_weight = (m_weight * value.m_weight);
}


template <class ValueT, class WeightT>
inline TWeightedNumber<ValueT, WeightT> TWeightedNumber<ValueT, WeightT>::GetSimpleAdd(const TWeightedNumber<ValueT, WeightT>& value) const
{
	TWeightedNumber<ValueT, WeightT> result;
	
	GetSimpleAdd(value, result);

	return result;
}


template <class ValueT, class WeightT>
inline void TWeightedNumber<ValueT, WeightT>::GetSimpleAdd(const TWeightedNumber<ValueT, WeightT>& value, TWeightedNumber<ValueT, WeightT>& result) const
{
	result.m_sum = (m_sum * value.m_weight + value.m_sum * m_weight);
	result.m_weight = (m_weight * value.m_weight);
}


template <class ValueT, class WeightT>
void TWeightedNumber<ValueT, WeightT>::SimpleSub(const TWeightedNumber<ValueT, WeightT>& value)
{
	m_sum = (m_sum * value.m_weight - value.m_sum * m_weight);
	m_weight = (m_weight * value.m_weight);
}


template <class ValueT, class WeightT>
TWeightedNumber<ValueT, WeightT> TWeightedNumber<ValueT, WeightT>::GetSimpleSub(const TWeightedNumber<ValueT, WeightT>& value) const
{
	TWeightedNumber<ValueT, WeightT> result;
	
	GetSimpleSub(value, result);

	return result;
}


template <class ValueT, class WeightT>
void TWeightedNumber<ValueT, WeightT>::GetSimpleSub(const TWeightedNumber<ValueT, WeightT>& value, TWeightedNumber<ValueT, WeightT>& result) const
{
	result.m_sum = (m_sum * value.m_weight - value.m_sum * m_weight);
	result.m_weight = (m_weight * value.m_weight);
}


template <class ValueT, class WeightT>
inline void TWeightedNumber<ValueT, WeightT>::Add(const TWeightedNumber<ValueT, WeightT>& value)
{
	SimpleAdd(value);
	Normalize(::sqrt(m_weight));
}


template <class ValueT, class WeightT>
inline TWeightedNumber<ValueT, WeightT> TWeightedNumber<ValueT, WeightT>::GetAdd(const TWeightedNumber<ValueT, WeightT>& value) const
{
	TWeightedNumber<ValueT, WeightT> result;

	GetSimpleAdd(value, result);
	result.Normalize(::sqrt(result.m_weight));

	return result;
}


template <class ValueT, class WeightT>
inline void TWeightedNumber<ValueT, WeightT>::GetAdd(const TWeightedNumber<ValueT, WeightT>& value, TWeightedNumber<ValueT, WeightT>& result) const
{
	GetSimpleAdd(value, result);
	result.Normalize(::sqrt(result.m_weight));
}


template <class ValueT, class WeightT>
inline void TWeightedNumber<ValueT, WeightT>::Sub(const TWeightedNumber<ValueT, WeightT>& value)
{
	SimpleSub(value);
	Normalize(::sqrt(m_weight));
}


template <class ValueT, class WeightT>
inline TWeightedNumber<ValueT, WeightT> TWeightedNumber<ValueT, WeightT>::GetSub(const TWeightedNumber<ValueT, WeightT>& value) const
{
	TWeightedNumber<ValueT, WeightT> result;

	GetSimpleSub(value, result);
	result.Normalize(::sqrt(result.m_weight));

	return result;
}


template <class ValueT, class WeightT>
inline void TWeightedNumber<ValueT, WeightT>::GetSub(const TWeightedNumber<ValueT, WeightT>& value, TWeightedNumber<ValueT, WeightT>& result) const
{
	GetSimpleSub(value, result);
	result.Normalize(::sqrt(result.m_weight));
}


template <class ValueT, class WeightT>
inline bool TWeightedNumber<ValueT, WeightT>::operator==(const TWeightedNumber<ValueT, WeightT>& number) const
{
	return			(m_sum == number.m_sum) &&
					(m_weight == number.m_weight);
}


template <class ValueT, class WeightT>
inline bool TWeightedNumber<ValueT, WeightT>::operator!=(const TWeightedNumber<ValueT, WeightT>& number) const
{
	return			(m_sum != number.m_sum) ||
					(m_weight != number.m_weight);
}


template <class ValueT, class WeightT>
inline TWeightedNumber<ValueT, WeightT> TWeightedNumber<ValueT, WeightT>::operator+(const TWeightedNumber<ValueT, WeightT>& value) const
{
	return TWeightedNumber<ValueT, WeightT>(m_sum + value.m_sum, m_weight + value.m_weight);
}


template <class ValueT, class WeightT>
inline TWeightedNumber<ValueT, WeightT> TWeightedNumber<ValueT, WeightT>::operator-(const TWeightedNumber<ValueT, WeightT>& value) const
{
	return TWeightedNumber<ValueT, WeightT>(m_sum - value.m_sum, m_weight - value.m_weight);
}


template <class ValueT, class WeightT>
inline TWeightedNumber<ValueT, WeightT> TWeightedNumber<ValueT, WeightT>::operator*(const TWeightedNumber<ValueT, WeightT>& value) const
{
	return TWeightedNumber<ValueT, WeightT>(m_sum * value.m_sum, m_weight * value.m_weight);
}


template <class ValueT, class WeightT>
inline TWeightedNumber<ValueT, WeightT> TWeightedNumber<ValueT, WeightT>::operator/(const TWeightedNumber<ValueT, WeightT>& value) const
{
	return TWeightedNumber<ValueT, WeightT>(m_sum / value.m_sum, m_weight / value.m_weight);
}


template <class ValueT, class WeightT>
inline const TWeightedNumber<ValueT, WeightT>& TWeightedNumber<ValueT, WeightT>::operator=(const TWeightedNumber<ValueT, WeightT>& value)
{
	m_sum = value.m_sum;
	m_weight = value.m_weight;

	return *this;
}


template <class ValueT, class WeightT>
inline const TWeightedNumber<ValueT, WeightT>& TWeightedNumber<ValueT, WeightT>::operator+=(const TWeightedNumber<ValueT, WeightT>& value)
{
	m_sum = value.m_sum + m_sum;
	m_weight = value.m_weight + m_weight;

	return *this;
}


template <class ValueT, class WeightT>
inline const TWeightedNumber<ValueT, WeightT>& TWeightedNumber<ValueT, WeightT>::operator-=(const TWeightedNumber<ValueT, WeightT>& value)
{
	m_sum = value.m_sum - m_sum;
	m_weight = value.m_weight - m_weight;

	return *this;
}


template <class ValueT, class WeightT>
inline const TWeightedNumber<ValueT, WeightT>& TWeightedNumber<ValueT, WeightT>::operator*=(const TWeightedNumber<ValueT, WeightT>& value)
{
	m_sum *= value.m_sum;
	m_weight *= value.m_weight;

	return *this;
}


template <class ValueT, class WeightT>
inline const TWeightedNumber<ValueT, WeightT>& TWeightedNumber<ValueT, WeightT>::operator/=(const TWeightedNumber<ValueT, WeightT>& value)
{
	m_sum /= value.m_sum;
	m_weight /= value.m_weight;

	return *this;
}


template <class ValueT, class WeightT>
inline imath::TWeightedNumber<ValueT, WeightT> TWeightedNumber<ValueT, WeightT>::operator+(ValueType scalar) const
{
	return imath::TWeightedNumber<ValueT, WeightT>(m_sum + m_weight * scalar, m_weight);
}


template <class ValueT, class WeightT>
inline imath::TWeightedNumber<ValueT, WeightT> TWeightedNumber<ValueT, WeightT>::operator-(ValueType scalar) const
{
	return imath::TWeightedNumber<ValueT, WeightT>(m_sum - m_weight * scalar, m_weight);
}


template <class ValueT, class WeightT>
inline TWeightedNumber<ValueT, WeightT> TWeightedNumber<ValueT, WeightT>::operator*(double scalar) const
{
	return TWeightedNumber<ValueT, WeightT>(ValueType(m_sum * scalar), m_weight);
}


template <class ValueT, class WeightT>
inline imath::TWeightedNumber<ValueT, WeightT> TWeightedNumber<ValueT, WeightT>::operator/(double scalar) const
{
	return imath::TWeightedNumber<ValueT, WeightT>(ValueType(m_sum / scalar), m_weight);
}


// typedefs

typedef TWeightedNumber<float> CWFloat;
typedef TWeightedNumber<double> CWDouble;


} // namespace imath


#endif // !imath_TWeightedNumber_included


