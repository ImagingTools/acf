#pragma once


// Qt includes
#include <QtCore/QByteArray>

// ACF includes
#include <istd/IPolymorphic.h>
#include <imath/TIValueManip.h>


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


