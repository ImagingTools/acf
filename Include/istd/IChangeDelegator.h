#ifndef istd_IChangeDelegator_included
#define istd_IChangeDelegator_included


#include "istd/IChangeable.h"


namespace istd
{


/**
	Common interface for all classes that supports delegation of their updates to
	another class. This is usefull for composed objects to delegate information about changes
	from a child element to its owner.
 */
class IChangeDelegator: virtual public istd::IChangeable
{
public:
	/**
		Sets the slave object pointer. 
		Its IChangeable methods will be called, if this object is updated.
	*/
	virtual istd::IChangeable* GetSlavePtr() const = 0;

	/**
		Gets the slave object pointer. 
	*/
	virtual void SetSlavePtr(istd::IChangeable* slavePtr) = 0;
};


} // namespace istd


#endif // !istd_IChangeDelegator_included


