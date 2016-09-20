#ifndef IAConstraints_included
#define IAConstraints_included


// ACF includes
#include <istd/IChangeable.h>
#include <istd/TRange.h>


/**
	Interface for contraints of A.
*/
class IAConstraints: virtual public istd::IChangeable
{
public:
	/**
		Get the possible value range for A.
	*/
	virtual const istd::CRange& GetARange() const = 0;
};


#endif // !IAConstraints_included


