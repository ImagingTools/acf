#ifndef istd_TChangeDelegator_included
#define istd_TChangeDelegator_included


#include "istd/istd.h"

#include "istd/CChangeDelegator.h"
#include "istd/TChangeBinder.h"


namespace istd
{


/**
	Binder of some istd::IChangeable implementation and changing delegator.
	\sa CChangeDelegator
*/
template <class Base>
class TChangeDelegator: public istd::TChangeBinder<Base, CChangeDelegator>
{
public:
	typedef istd::TChangeBinder<Base, CChangeDelegator> BaseClass;

	TChangeDelegator();
	explicit TChangeDelegator(istd::IChangeable* slavePtr);
};


// public methods

template <class Base>
TChangeDelegator<Base>::TChangeDelegator()
{
}


template <class Base>
TChangeDelegator<Base>::TChangeDelegator(IChangeable* slavePtr)
{
	SetSlavePtr(slavePtr);
}


} // namespace istd


#endif // !istd_TChangeDelegator_included

