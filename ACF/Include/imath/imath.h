#ifndef imath_included
#define imath_included


#include "iser/iser.h"


/**
\defgroup imath Mathematics
\ingroup ACF
@{
*/

/** 
@} 
*/


/**
	Package with mathematical functions and algebraical primitives.
	This package is system independent.
	This package use following ACF packages:
	\li istd
	\li iser
*/
namespace imath
{


} // namespace imath


static const double I_PI = 3.1415926;


inline double GetRadianFromDegree(double degree)
{
	return degree / 180.0 * I_PI;
}


inline double GetDegreeFromRadian(double radian)
{
	return radian * 180.0 / I_PI;
}



#endif // !imath_included

