#ifndef istd_TChangeDelegator_included
#define istd_TChangeDelegator_included


// ACF includes
#include <istd/CChangeDelegator.h>


namespace istd
{


/**
	Binder of some istd::IChangeable implementation and changing delegator.
	\sa CChangeDelegator

	\ingroup DataModel
*/
template <class Base>
class TChangeDelegator:
			public Base,
			public CChangeDelegator
{
public:
	typedef Base BaseClass;
	typedef CChangeDelegator BaseClass2;

	TChangeDelegator();
	explicit TChangeDelegator(istd::IChangeable* slavePtr);

	// reimplemented (istd::IChangeable)
	virtual void OnBeginChanges();
	virtual void OnEndChanges(const istd::IChangeable::ChangeSet& changeSet);
};


// public methods

template <class Base>
TChangeDelegator<Base>::TChangeDelegator()
{
}


template <class Base>
TChangeDelegator<Base>::TChangeDelegator(IChangeable* slavePtr)
{
	BaseClass2::SetSlavePtr(slavePtr);
}


// pseudo-reimplemented (istd::IChangeable)

template <class Base>
void TChangeDelegator<Base>::OnBeginChanges()
{
	BaseClass2::OnBeginChanges();

	BaseClass::OnBeginChanges();
}


template <class Base>
void TChangeDelegator<Base>::OnEndChanges(const istd::IChangeable::ChangeSet& changeSet)
{
	BaseClass::OnEndChanges(changeSet);

	BaseClass2::OnEndChanges(changeSet);
}


} // namespace istd


#endif // !istd_TChangeDelegator_included

