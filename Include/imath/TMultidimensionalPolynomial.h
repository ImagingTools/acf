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
	result = TMultidimensionalPolynomial::GetValueAt(argument);

	return true;
}


template <int Dimensions, class Element>
typename TMultidimensionalPolynomial<Dimensions, Element>::ResultType TMultidimensionalPolynomial<Dimensions, Element>::GetValueAt(const ArgumentType& argument) const
{
	typename BaseClass::ResultType retVal;

	istd::TIndex<Dimensions> index;
	CumulateRecursiveValueAt(argument, Dimensions - 1, index, retVal);

	return retVal;
}


template <>
inline typename TMultidimensionalPolynomial<1, double>::ResultType TMultidimensionalPolynomial<1, double>::GetValueAt(const ArgumentType& argument) const
{
	double partArgument = argument[0];
	int dimensionSize = m_coefficients.GetSize(0);

	ResultType result = 0;

	istd::TIndex<1> index;
	int& indexElement = index[0];
	for (indexElement = dimensionSize - 1; indexElement >= 0; --indexElement){
		result = result * partArgument + m_coefficients.GetAt(index);
	}

	return result;
}


template <>
inline typename TMultidimensionalPolynomial<2, double>::ResultType TMultidimensionalPolynomial<2, double>::GetValueAt(const ArgumentType& argument) const
{
	double partArgument1 = argument[0];
	double partArgument2 = argument[1];

	int dimensionSize1 = m_coefficients.GetSize(0);
	int dimensionSize2 = m_coefficients.GetSize(1);

	ResultType result = 0;

	istd::TIndex<2> index;
	int& indexElement1 = index[0];
	int& indexElement2 = index[1];

	if (dimensionSize1 == 1){
		indexElement1 = 0;
		for (indexElement2 = dimensionSize2 - 1; indexElement2 >= 0; --indexElement2){
			result = result * partArgument2 + m_coefficients.GetAt(index);
		}
	}
	else{
		for (indexElement2 = dimensionSize2 - 1; indexElement2 >= 0; --indexElement2){
			indexElement1 = dimensionSize1 - 1;
			ResultType partResult = m_coefficients.GetAt(index);

			for (indexElement1 = dimensionSize1 - 2; indexElement1 >= 0; --indexElement1){
				partResult = partResult * partArgument1 + m_coefficients.GetAt(index);
			}

			result = result * partArgument2 + partResult;
		}
	}

	return result;
}


template <>
inline typename TMultidimensionalPolynomial<3, double>::ResultType TMultidimensionalPolynomial<3, double>::GetValueAt(const ArgumentType& argument) const
{
	double partArgument1 = argument[0];
	double partArgument2 = argument[1];
	double partArgument3 = argument[2];

	int dimensionSize1 = m_coefficients.GetSize(0);
	int dimensionSize2 = m_coefficients.GetSize(1);
	int dimensionSize3 = m_coefficients.GetSize(2);

	ResultType result = 0;

	istd::TIndex<3> index;
	int& indexElement1 = index[0];
	int& indexElement2 = index[1];
	int& indexElement3 = index[2];

	if (dimensionSize1 == 1){
		indexElement1 = 0;

		for (indexElement3 = dimensionSize3 - 1; indexElement3 >= 0; --indexElement3){
			ResultType partResult = 0;
			for (indexElement2 = dimensionSize2 - 1; indexElement2 >= 0; --indexElement2){
				partResult = partResult * partArgument2 + m_coefficients.GetAt(index);
			}
			result = result * partArgument3 + partResult;
		}
	}
	else{
		for (indexElement3 = dimensionSize3 - 1; indexElement3 >= 0; --indexElement3){
			ResultType partResult2 = 0;

			for (indexElement2 = dimensionSize2 - 1; indexElement2 >= 0; --indexElement2){
				indexElement1 = dimensionSize1 - 1;
				ResultType partResult = m_coefficients.GetAt(index);

				for (indexElement1 = dimensionSize1 - 2; indexElement1 >= 0; --indexElement1){
					partResult = partResult * partArgument1 + m_coefficients.GetAt(index);
				}

				partResult2 = partResult2 * partArgument2 + partResult;
			}
			result = result * partArgument3 + partResult2;
		}
	}

	return result;
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


