#ifndef imath_IDoubleManip_included
#define imath_IDoubleManip_included


// STL includes
#include <string>


// ACF includes
#include "istd/IPolymorphic.h"

#include "imath/TIValueManip.h"


namespace imath
{


/**
	Interface for all manipulation using values represent as \c double.
*/
class IDoubleManip: virtual public TIValueManip<double>
{
public:
	/**
		Get number of digits after point.
	*/
	virtual int GetPrecision() const = 0;
};


} // namespace imath


#endif // !imath_IDoubleManip_included


