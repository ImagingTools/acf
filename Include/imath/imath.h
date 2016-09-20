#ifndef imath_included
#define imath_included


#include <iser/iser.h>


/**
	Mathematical constant value PI.
*/
static const double I_PI = 3.14159265358979323846;

/**
	Mathematical constant value 2 * PI.
*/
static const double I_2PI = 6.2831853071795864769;


/**
	Package with mathematical functions and algebraical primitives.
	This package is system independent.
	This package use following ACF packages:
	\li istd
	\li iser

	\ingroup NumericalBase
*/
namespace imath
{


inline double GetRadianFromDegree(double degree)
{
	return degree / 180.0 * I_PI;
}


inline double GetDegreeFromRadian(double radian)
{
	return radian * 180.0 / I_PI;
}


} // namespace imath


#endif // !imath_included

