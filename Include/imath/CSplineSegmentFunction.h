#pragma once


namespace imath
{


/**
	Helper class for calculation of splines of 3rd degree.
*/
class CSplineSegmentFunction
{
public:
	// static methods
	/**
		Get kernel of value normalized to range [0, 1].
	*/
	static double GetValueKernelAt(double alpha);
	/**
		Get kernel of derivative normalized to range [0, 1].
	*/
	static double GetDerivativeKernelAt(double alpha);
};


// static protected inline methods

inline double CSplineSegmentFunction::GetValueKernelAt(double alpha)
{
	return (2 * alpha - 3) * alpha * alpha + 1;
}


inline double CSplineSegmentFunction::GetDerivativeKernelAt(double alpha)
{
	double beta = 1 - alpha;
	return alpha * beta * beta;
}


} // namespace imath


