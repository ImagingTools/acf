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

	typedef Degree DerivativeDegreeType;

	// reimplemented (imath::TIMathFunction<Argument, Result>)
	virtual bool GetValueAt(const Argument& argument, Result& result) const;
	virtual Result GetValueAt(const Argument& argument) const;

protected:
	/**
		Cumulate interpolated value or derivative at specified recursion level.
		\param	argument			position, where interpolation should be calculated.
									The elements with index less of equal \c dimension indicate non grid components (hard to explain).
									The other components should be taken from index.
		\param	sizes				size fulcrum grid.
		\param	dimension			working dimension and recursion level.
		\param	index				multidimensional index pointing at cuboid element in fulcrum grid.
									The elements with index greater \c dimension indicate asked subspace in grid (hard to explain).
									For the sake of performance it is internal modified,
									but original state is reverted before this method returns.
		\param	degree				derivative degree will be taken.
									For the sake of performance it is internal modified,
									but original state is reverted before this method returns.
		\param	cumulationFactor	scaled factor for cumulation.
		\param	result				object where values are cumulated.
	*/
	void CumulateRecursiveValueAt(
				const Argument& argument,
				int dimension,
				const typename BaseClass::FulcrumSizes& sizes,
				typename BaseClass::FulcrumIndex& index,
				DerivativeDegreeType& degree,
				double cumulationFactor,
				Result& result) const;

	// static methods
	static double GetValueKernelAt(double alpha);
	static double GetDerivativeKernelAt(double alpha);

	// abstract methods
	/**
		Get derivative of specified degree at specified index position.
	*/
	virtual const typename BaseClass::ResultType& GetFulcrumDerivativeAtIndex(
				const typename BaseClass::FulcrumIndex& index,
				const DerivativeDegreeType& degree) const = 0;

	/**
		Check, if this or higher degree derrivatives are supported.
	*/
	virtual bool IsDerivativeDegreeSupported(const DerivativeDegreeType& degree) const = 0;
};


// static protected inline methods

template <class Argument, class Result, class Fulcrums, class Degree>
inline double TSplineGridFunctionBase<Argument, Result, Fulcrums, Degree>::GetValueKernelAt(double alpha)
{
	return (2 * alpha - 3) * alpha * alpha + 1;
}


template <class Argument, class Result, class Fulcrums, class Degree>
inline double TSplineGridFunctionBase<Argument, Result, Fulcrums, Degree>::GetDerivativeKernelAt(double alpha)
{
	double beta = 1 - alpha;
	return alpha * beta * beta;
}


// public methods

// reimplemented (imath::TIMathFunction<Argument, Result>)

template <class Argument, class Result, class Fulcrums, class Degree>
bool TSplineGridFunctionBase<Argument, Result, Fulcrums, Degree>::GetValueAt(const Argument& argument, Result& result) const
{
	result.Clear();

	if (BaseClass::EnsureCacheValid()){
		typename BaseClass::FulcrumIndex index = this->FindIndices(argument);

		typename BaseClass::FulcrumSizes gridSize = BaseClass::GetGridSize();

		if (index.IsInside(gridSize)){
			int dimensionsCount = BaseClass::GetDimensionsCount();
			Degree degree;
			degree.SetDimensionsCount(dimensionsCount);

			CumulateRecursiveValueAt(argument, dimensionsCount - 1, gridSize, index, degree, 1.0, result);

			return true;
		}
	}

	return false;
}


template <class Argument, class Result, class Fulcrums, class Degree>
Result TSplineGridFunctionBase<Argument, Result, Fulcrums, Degree>::GetValueAt(const Argument& argument) const
{
	typename BaseClass::ResultType retVal;

	GetValueAt(argument, retVal);

	return retVal;
}


// protected methods

template <class Argument, class Result, class Fulcrums, class Degree>
void TSplineGridFunctionBase<Argument, Result, Fulcrums, Degree>::CumulateRecursiveValueAt(
			const Argument& argument,
			int dimension,
			const typename BaseClass::FulcrumSizes& sizes,
			typename BaseClass::FulcrumIndex& index,
			DerivativeDegreeType& derivativeDegree,
			double cumulationFactor,
			Result& result) const
{
	Q_ASSERT(dimension < BaseClass::GetDimensionsCount());
	Q_ASSERT(sizes.GetDimensionsCount() == BaseClass::GetDimensionsCount());
	Q_ASSERT(index.GetDimensionsCount() == BaseClass::GetDimensionsCount());

	if (dimension < 0){
		result.ScaledCumulate(GetFulcrumDerivativeAtIndex(index, derivativeDegree), cumulationFactor);

		return;
	}

	int& indexElement = index[dimension];

	if (indexElement >= 0){
		if (indexElement < sizes[dimension] - 1){
			double firstPosition = BaseClass::GetLayerPosition(dimension, indexElement);
			double secondPosition = BaseClass::GetLayerPosition(dimension, indexElement + 1);
			double layersDistance = secondPosition - firstPosition;
			Q_ASSERT(layersDistance >= 0);
			Q_ASSERT(argument[dimension] >= firstPosition);
			Q_ASSERT(argument[dimension] <= secondPosition);
			Q_ASSERT(derivativeDegree[dimension] == 0);

			bool useDerivative = false;
			if (derivativeDegree.IncreaseAt(dimension)){
				useDerivative = IsDerivativeDegreeSupported(derivativeDegree);
				derivativeDegree.DecreaseAt(dimension);
			}

			double alpha = (argument[dimension] - firstPosition) / layersDistance;

			double firstValueFactor = useDerivative? GetValueKernelAt(alpha): (1 - alpha);	// use linear interpolation if no derivative is available
			if (firstValueFactor > I_BIG_EPSILON){
				CumulateRecursiveValueAt(
							argument,
							dimension - 1,
							sizes,
							index,
							derivativeDegree,
							cumulationFactor * firstValueFactor,
							result);
			}

			double secondValueFactor = useDerivative? GetValueKernelAt(1.0 - alpha): alpha;	// use linear interpolation if no derivative is available

			if (secondValueFactor > I_BIG_EPSILON){
				++indexElement;
				CumulateRecursiveValueAt(
							argument,
							dimension - 1,
							sizes,
							index,
							derivativeDegree,
							cumulationFactor * secondValueFactor,
							result);
				--indexElement;
			}

			if (useDerivative){
				derivativeDegree.IncreaseAt(dimension);

				double firstDerivativeFactor = GetDerivativeKernelAt(alpha) * layersDistance;
				if (firstDerivativeFactor > I_BIG_EPSILON){
					CumulateRecursiveValueAt(
								argument,
								dimension - 1,
								sizes,
								index,
								derivativeDegree,
								cumulationFactor * firstDerivativeFactor,
								result);
				}

				++indexElement;

				double secondDerivativeFactor = -GetDerivativeKernelAt(1.0 - alpha) * layersDistance;
				if (secondDerivativeFactor < -I_BIG_EPSILON){
					CumulateRecursiveValueAt(
								argument,
								dimension - 1,
								sizes,
								index,
								derivativeDegree,
								cumulationFactor * secondDerivativeFactor,
								result);
				}

				--indexElement;

				derivativeDegree.DecreaseAt(dimension);
			}
		}
		else{
			// element out of boundaries at this dimension
			Q_ASSERT(indexElement == sizes[dimension] - 1);

			CumulateRecursiveValueAt(
						argument,
						dimension - 1,
						sizes,
						index,
						derivativeDegree,
						cumulationFactor,
						result);
		}
	}
	else{
		// element out of boundaries at this dimension
		Q_ASSERT(indexElement == -1);

		++indexElement;
		CumulateRecursiveValueAt(
					argument,
					dimension - 1,
					sizes,
					index,
					derivativeDegree,
					cumulationFactor,
					result);
		--indexElement;
	}
}


} // namespace imath


#endif // !imath_TSplineGridFunctionBase_included


