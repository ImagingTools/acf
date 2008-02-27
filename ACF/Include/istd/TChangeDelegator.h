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
template <class BaseClass>
class TChangeDelegator: public istd::TChangeBinder<BaseClass, CChangeDelegator>
{
public:
	typedef CChangeDelegator BaseClass2;

	TChangeDelegator();
	explicit TChangeDelegator(istd::IChangeable* slavePtr);
};


// public methods

template <class BaseClass>
TChangeDelegator<BaseClass>::TChangeDelegator()
{
}


template <class BaseClass>
TChangeDelegator<BaseClass>::TChangeDelegator(IChangeable* slavePtr)
	:BaseClass2(slavePtr)
{
}


} // namespace istd


#endif // !istd_TChangeDelegator_included

