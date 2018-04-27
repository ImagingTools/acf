#ifndef imath_TMultidimensionalPolynomial_included
#define imath_TMultidimensionalPolynomial_included


// ACF includes
#include <istd/TArray.h>
#include <imath/TVector.h>
#include <imath/TFulcrumGridFunctionBase.h>
#include <imath/CSplineSegmentFunction.h>


namespace imath
{


/**
	Multidimensional polynomial function.
	\param	Dimensions	number of dimensions.
	\param	Element		type of result value.
*/
template <int Dimensions, class Element = double>
class TMultidimensionalPolynomial: public imath::TIMathFunction<TVector<Dimensions>, Element>
{
public:
	typedef imath::TIMathFunction<TVector<Dimensions>, Element> BaseClass;
	typedef istd::TArray<Element, Dimensions> Coefficients;

	TMultidimensionalPolynomial();
	explicit TMultidimensionalPolynomial(const Coefficients& coefficients);

	const Coefficients& GetCoefficients() const;
	void SetCoefficients(const Coefficients& coefficients);

	// reimplemented (imath::TIMathFunction)
	virtual bool GetValueAt(const ArgumentType& argument, ResultType& result) const;
	virtual ResultType GetValueAt(const ArgumentType& argument) const;

	// static public methods
	/**
		Get value of base function for single coefficient.
	*/
	static double GetBaseFunctionValue(const ArgumentType& argument, const istd::TIndex<Dimensions>& coeffIndex);

protected:
	/**
		Cumulate interpolated value or derivative at specified recursion level.
		\param	argument			position, where interpolation should be calculated.
		\param	dimension			working dimension and recursion level.
		\param	index				multidimensional index pointing at coefficient.
									For the sake of performance it is internal modified.
		\param	result				output value.
	*/
	void CumulateRecursiveValueAt(
				const ArgumentType& argument,
				int dimension,
				typename istd::TIndex<Dimensions>& index,
				ResultType& result) const;

private:
	istd::TArray<Element, Dimensions> m_coefficients;
};


// public methods

template <int Dimensions, class Element>
TMultidimensionalPolynomial<Dimensions, Element>::TMultidimensionalPolynomial()
{
}


template <int Dimensions, class Element>
TMultidimensionalPolynomial<Dimensions, Element>::TMultidimensionalPolynomial(const Coefficients& coefficients)
:	m_coefficients(coefficients)
{
}


template <int Dimensions, class Element>
typename const TMultidimensionalPolynomial<Dimensions, Element>::Coefficients& TMultidimensionalPolynomial<Dimensions, Element>::GetCoefficients() const
{
	return m_coefficients;
}


template <int Dimensions, class Element>
void TMultidimensionalPolynomial<Dimensions, Element>::SetCoefficients(const Coefficients& coefficients)
{
	m_coefficients = coefficients;
}


// reimplemented (imath::TIMathFunction)

template <int Dimensions, class Element>
bool TMultidimensionalPolynomial<Dimensions, Element>::GetValueAt(const ArgumentType& argument, ResultType& result) const
{
	istd::TIndex<Dimensions> index;
	CumulateRecursiveValueAt(argument, Dimensions - 1, index, result);

	return true;
}


template <int Dimensions, class Element>
typename TMultidimensionalPolynomial<Dimensions, Element>::ResultType TMultidimensionalPolynomial<Dimensions, Element>::GetValueAt(const ArgumentType& argument) const
{
	typename BaseClass::ResultType retVal;

	GetValueAt(argument, retVal);

	return retVal;
}


// static public methods

template <int Dimensions, class Element>
double TMultidimensionalPolynomial<Dimensions, Element>::GetBaseFunctionValue(const ArgumentType& argument, const istd::TIndex<Dimensions>& coeffIndex)
{
	Element retVal = 1;

	for (int dimension = 0; dimension < Dimensions; ++dimension){
		retVal *= std::pow(argument.GetElement(dimension), coeffIndex.GetAt(dimension));
	}

	return retVal;
}


// protected methods

template <int Dimensions, class Element>
void TMultidimensionalPolynomial<Dimensions, Element>::CumulateRecursiveValueAt(
			const ArgumentType& argument,
			int dimension,
			typename istd::TIndex<Dimensions>& index,
			ResultType& result) const
{
	Q_ASSERT(dimension < Dimensions);
	Q_ASSERT(dimension >= 0);

	int& indexElement = index[dimension];
	int dimensionSize = m_coefficients.GetSize(dimension);

	double partArgument = argument[dimension];

	result = 0;

	if (dimension <= 0){
		for (indexElement = dimensionSize - 1; indexElement >= 0; --indexElement){
			result = result * partArgument + m_coefficients.GetAt(index);
		}
	}
	else{
		for (indexElement = dimensionSize - 1; indexElement >= 0; --indexElement){
			ResultType partResult;

			CumulateRecursiveValueAt(argument, dimension - 1, index, partResult);

			result = result * partArgument + partResult;
		}
	}
}


} // namespace imath


#endif // !imath_TMultidimensionalPolynomial_included


