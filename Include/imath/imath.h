#ifndef imath_included
#define imath_included


#include "iser/iser.h"


/**
	Mathematical constant value PI.

	\ingroup Main
*/
static const double I_PI = 3.14159265358979323846;


/**
	Package with mathematical functions and algebraical primitives.
	This package is system independent.
	This package use following ACF packages:
	\li istd
	\li iser

	\ingroup Main
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

