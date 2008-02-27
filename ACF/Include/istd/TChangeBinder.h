#ifndef istd_TChangeBinder_included
#define istd_TChangeBinder_included


#include "istd/istd.h"

#include "istd/CChangeDelegator.h"


namespace istd
{


/**
	Binder of two istd::IChangeable implementations
	\sa CChangeDelegator
*/
template <class BaseClass, class BaseClass2>
class TChangeBinder: public BaseClass, public BaseClass2
{
public:
	// pseudo-reimplemented (istd::IChangeable)
	virtual void BeginChanges(int changeFlags = 0, istd::IPolymorphic* changeParamsPtr = NULL);
	virtual void EndChanges(int changeFlags = 0, istd::IPolymorphic* changeParamsPtr = NULL);
};


// pseudo-reimplemented (istd::IChangeable)

template <class BaseClass, class BaseClass2>
void TChangeBinder<BaseClass, BaseClass2>::BeginChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr)
{
	BaseClass::BeginChanges(changeFlags, changeParamsPtr);
	BaseClass2::BeginChanges(changeFlags, changeParamsPtr);
}


template <class BaseClass, class BaseClass2>
void TChangeBinder<BaseClass, BaseClass2>::EndChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr)
{
	BaseClass::EndChanges(changeFlags, changeParamsPtr);
	BaseClass2::EndChanges(changeFlags, changeParamsPtr);
}


} // namespace istd


#endif // !istd_TChangeBinder_included

