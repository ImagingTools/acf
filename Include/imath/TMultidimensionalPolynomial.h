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
	typedef typename Coefficients::SizesType CoeffGridSize;

	TMultidimensionalPolynomial();
	explicit TMultidimensionalPolynomial(const Coefficients& coefficients);

	const Coefficients& GetCoefficients() const;
	void SetCoefficients(const Coefficients& coefficients);

	bool ApproximateCoefficientsFromFulcrums(const CoeffGridSize& coeffGridSize, const ArgumentType* arguments, ResultType* destValues, int count);

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


template <int Dimensions, class Element>
bool TMultidimensionalPolynomial<Dimensions, Element>::ApproximateCoefficientsFromFulcrums(
			const CoeffGridSize& coeffGridSize,
			const ArgumentType* arguments,
			ResultType* destValues,
			int count)
{
	m_coefficients.SetSizes(coeffGridSize);

	if (coeffGridSize.IsSizeEmpty()){
		return true;
	}

	imath::CVarMatrix valueMatrix(istd::CIndex2d(coeffGridSize.GetProductVolume(), count));
	imath::CVarMatrix destValueVector(istd::CIndex2d(1, count));

	typedef imath::TMultidimensionalPolynomial<2, double> Polynomial;

	for (int matrixRow = 0; matrixRow < count; ++matrixRow){
		const ArgumentType& argument = arguments[matrixRow];
		double measValue = double(destValues[matrixRow]);

		int matrixCol = 0;
		CoeffGridSize coeffIndex = CoeffGridSize::GetZero();
		do{
			double baseValue = GetBaseFunctionValue(argument, coeffIndex);

			valueMatrix.SetAt(istd::CIndex2d(matrixCol++, matrixRow), baseValue);
		} while (coeffIndex.Increase(coeffGridSize));

		destValueVector.SetAt(istd::CIndex2d(0, matrixRow), measValue);
	}

	imath::CVarMatrix resultMatrix = imath::CVarMatrix(istd::CIndex2d(coeffGridSize.GetProductVolume(), 1));
	resultMatrix.Clear();

	if (!valueMatrix.GetSolvedLSP(destValueVector, resultMatrix)){
		return false;
	}
	Q_ASSERT(resultMatrix.GetSizes() == istd::CIndex2d(1, coeffGridSize.GetProductVolume()));

	m_coefficients.SetSizes(coeffGridSize);

	int resultRow = 0;
	CoeffGridSize coeffIndex = CoeffGridSize::GetZero();
	do{
		m_coefficients.SetAt(coeffIndex, resultMatrix.GetAt(istd::CIndex2d(0, resultRow++)));
	} while (coeffIndex.Increase(coeffGridSize));

	return true;
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


