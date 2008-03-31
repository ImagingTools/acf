#ifndef imath_TSplineGridFunctionBase_included
#define imath_TSplineGridFunctionBase_included


#include "istd/TArray.h"

#include "imath/TVector.h"
#include "imath/TFulcrumGridFunctionBase.h"


namespace imath
{


/**
	Spline interpolation function using polynomial 3 degree segments.
	To reduce number of coefficients only precalculated derrivative values for each fulcrum will be stored.
	\param	Argument	function argument type.
	\param	Result		function result type.
	\param	Fulcrums	array type used to stored fulcrums. \sa imath::TFulcrumGridFunctionBase.
	\param	Degree		type of object storing index of derivative degree.
						It should implement operator[] to access drivative degree for specified dimension.
*/
template <class Argument, class Result, class Fulcrums, class Degree>
class TSplineGridFunctionBase: public TFulcrumGridFunctionBase<Argument, Result, Fulcrums>
{
public:
	typedef TFulcrumGridFunctionBase<Argument, Result, Fulcrums> BaseClass;

	typedef Degree DerivativeDegree;

	// reimplemented (imath::TIMathFunction<Argument, Result>)
	virtual bool GetValueAt(const Argument& argument, Result& result) const;
	virtual Result GetValueAt(const Argument& argument) const;

protected:
	/**
		Calculate interpolated value or derivative at specified recursion level.
		\param	argument			position, where interpolation should be calculated.
		\param	sizes				size fulcrum grid.
		\param	dimension			working dimension and recursion level.
		\param	index				multidimensional index pointing at cuboid element in fulcrum grid.
									For the sake of performance it is internal modified,
									but original state is reverted before this method returns.
		\param	degree				derivative degree will be taken.
									For the sake of performance it is internal modified,
									but original state is reverted before this method returns.
		\param	result				returned object.
	*/
	void CalcRecursiveValueAt(
				const Argument& argument,
				int dimension,
				const FulcrumSizes& sizes,
				FulcrumIndex& index,
				DerivativeDegree& degree,
				Result& result) const;

	// static methods
	static double GetValueKernelAt(double alpha);
	static double GetDerivativeKernelAt(double alpha);

	// abstract methods
	/**
		Get derivative of specified degree at specified index position.
	*/
	virtual const ResultType& GetFulcrumDerivativeAtIndex(
				const FulcrumIndex& index,
				const DerivativeDegree& degree) const = 0;

	/**
		Check, if this or higher degree derrivatives are supported.
	*/
	virtual bool IsDerivativeDegreeSupported(const DerivativeDegree& degree) const = 0;
};


// static protected inline methods

template <class Argument, class Result, class Fulcrums, class Degree>
inline double TSplineGridFunctionBase<Argument, Result, Fulcrums, Degree>::GetValueKernelAt(double alpha)
{
	return 2 * alpha * alpha * alpha - 3 * alpha * alpha + 1;
}


template <class Argument, class Result, class Fulcrums, class Degree>
inline double TSplineGridFunctionBase<Argument, Result, Fulcrums, Degree>::GetDerivativeKernelAt(double alpha)
{
	return alpha * alpha * alpha - 2 * alpha * alpha + alpha;
}


// public methods

// reimplemented (imath::TIMathFunction<Argument, Result>)

template <class Argument, class Result, class Fulcrums, class Degree>
bool TSplineGridFunctionBase<Argument, Result, Fulcrums, Degree>::GetValueAt(const Argument& argument, Result& result) const
{
	if (EnsureCacheValid()){
		FulcrumIndex index = FindIndices(argument);

		Degree degree;
		degree.Reset();

		CalcRecursiveValueAt(argument, GetDimensionsCount() - 1, GetGridSize(), index, degree, result);

		return true;
	}
	else{
		return false;
	}
}


template <class Argument, class Result, class Fulcrums, class Degree>
typename Result TSplineGridFunctionBase<Argument, Result, Fulcrums, Degree>::GetValueAt(const Argument& argument) const
{
	ResultType retVal;

	GetValueAt(argument, retVal);

	return retVal;
}


// protected methods

template <class Argument, class Result, class Fulcrums, class Degree>
void TSplineGridFunctionBase<Argument, Result, Fulcrums, Degree>::CalcRecursiveValueAt(
			const Argument& argument,
			int dimension,
			const FulcrumSizes& sizes,
			FulcrumIndex& index,
			DerivativeDegree& derivativeDegree,
			Result& result) const
{
	I_ASSERT(dimension < GetDimensionsCount());
	I_ASSERT(sizes.GetDimensionsCount() == GetDimensionsCount());
	I_ASSERT(index.GetDimensionsCount() == GetDimensionsCount());

	if (dimension < 0){
		result = GetFulcrumDerivativeAtIndex(index, derivativeDegree);

		return;
	}

	int& indexElement = index[dimension];

	if (indexElement >= sizes[dimension] - 1){
		// element out of boundaries at this dimension
		I_ASSERT(indexElement == sizes[dimension] - 1);

		CalcRecursiveValueAt(argument, dimension - 1, sizes, index, derivativeDegree, result);
	}
	else if (indexElement < 0){
		// element out of boundaries at this dimension
		I_ASSERT(indexElement == -1);

		++indexElement;
		CalcRecursiveValueAt(argument, dimension - 1, sizes, index, derivativeDegree, result);
		--indexElement;
	}
	else{
		double firstPosition = GetLayerPosition(dimension, indexElement);
		double secondPosition = GetLayerPosition(dimension, indexElement + 1);
		double layersDistance = secondPosition - firstPosition;
		I_ASSERT(layersDistance >= 0);
		I_ASSERT(argument[dimension] >= firstPosition);
		I_ASSERT(argument[dimension] <= secondPosition);
		I_ASSERT(derivativeDegree[dimension] == 0);

		++derivativeDegree[dimension];
		bool useDerivative = IsDerivativeDegreeSupported(derivativeDegree);
		--derivativeDegree[dimension];

		double alpha = (argument[dimension] - firstPosition) / layersDistance;

		double firstValueFactor = useDerivative? GetValueKernelAt(alpha): (1 - alpha);	// use linear interpolation if no derivative is available

		Result firstValue;
		if (firstValueFactor > I_EPSILON){
			CalcRecursiveValueAt(argument, dimension - 1, sizes, index, derivativeDegree, firstValue);
		}
		else{
			firstValue.Clear();
		}

		++indexElement;

		double secondValueFactor = useDerivative? GetValueKernelAt(1.0 - alpha): alpha;	// use linear interpolation if no derivative is available

		Result secondValue;
		if (secondValueFactor > I_EPSILON){
			CalcRecursiveValueAt(argument, dimension - 1, sizes, index, derivativeDegree, secondValue);
		}
		else{
			secondValue.Clear();
		}

		--indexElement;

		if (useDerivative){
			++derivativeDegree[dimension];

			double firstDerivativeFactor = GetDerivativeKernelAt(alpha) * layersDistance;

			Result firstDerivative;
			if (firstDerivativeFactor > I_EPSILON){
				CalcRecursiveValueAt(argument, dimension - 1, sizes, index, derivativeDegree, firstDerivative);
			}
			else{
				firstDerivative.Clear();
			}

			++indexElement;

			double secondDerivativeFactor = GetDerivativeKernelAt(1.0 - alpha) * layersDistance;

			Result secondDerivative;
			if (secondDerivativeFactor > I_EPSILON){
				CalcRecursiveValueAt(argument, dimension - 1, sizes, index, derivativeDegree, secondDerivative);
			}
			else{
				secondDerivative.Clear();
			}

			--indexElement;

			--derivativeDegree[dimension];

			result =	firstValue * firstValueFactor +
						secondValue * secondValueFactor +
						firstDerivative * firstDerivativeFactor +
						secondDerivative * secondDerivativeFactor;
		}
		else{
			result =	firstValue * firstValueFactor +
						secondValue * secondValueFactor;
		}
	}
}


} // namespace imath


#endif // !imath_TSplineGridFunctionBase_included


