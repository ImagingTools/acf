#ifndef istd_TChangeBinder_included
#define istd_TChangeBinder_included


#include "istd/IPolymorphic.h"


namespace istd
{


/**
	Binder of two istd::IChangeable implementations
	\sa CChangeDelegator
*/
template <class Base, class Base2>
class TChangeBinder: public Base, public Base2
{
public:
	typedef Base BaseClass;
	typedef Base2 BaseClass2;

	// pseudo-reimplemented (istd::IChangeable)
	virtual void BeginChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr = NULL);
	virtual void EndChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr = NULL);
};


// pseudo-reimplemented (istd::IChangeable)

template <class Base, class Base2>
void TChangeBinder<Base, Base2>::BeginChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr)
{
	Base::BeginChanges(changeFlags, changeParamsPtr);
	Base2::BeginChanges(changeFlags, changeParamsPtr);
}


template <class Base, class Base2>
void TChangeBinder<Base, Base2>::EndChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr)
{
	Base::EndChanges(changeFlags, changeParamsPtr);
	Base2::EndChanges(changeFlags, changeParamsPtr);
}


} // namespace istd


#endif // !istd_TChangeBinder_included


