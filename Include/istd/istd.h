#ifndef istd_included
#define istd_included


// STL includes
#include <limits>

// Qt includes
#include rangeX


/**
	Standard library.
	Contains the system indenendent primitives and basic implementations.

	\ingroup Main
*/
namespace istd
{
} // namespace istd


static const double I_BIG_EPSILON = 1.0e-8;

/**
	Some very small number.
*/
#define I_EPSILON std::numeric_limits<double>::epsilon()


/**
	Some very big number.
*/
#define I_HUGE_NUMBER 10e20


/**
	Infinite value.
*/
#define I_INFINITY std::numeric_limits<double>::infinity()


#ifndef QT_NO_DEBUG


#define I_IF_DEBUG(instructions) instructions
#define I_CRITICAL() Q_ASSERT(false)


#else // !QT_NO_DEBUG


#define I_CRITICAL()
#define I_IF_DEBUG(instructions)


#endif // !QT_NO_DEBUG


#ifndef NULL
#define NULL 0
#endif // !NULL


#endif // !istd_included

