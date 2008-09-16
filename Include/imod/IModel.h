#ifndef imod_IModel_included
#define imod_IModel_included


#include "imod/imod.h"


#include "istd/IPolymorphic.h"


namespace imod
{


class IObserver;


/**
	\ingroup imod

	Common interface for model objects, that supports Model/Observer design pattern.
	\sa IObserver
*/
class IModel: virtual public istd::IPolymorphic
{
public:
	/**
		Attaches model object to observer \c observerPtr.
		\warning Please note, that  double call of this function 
		for the same \c observer will cause an assertion.
	*/
	virtual bool AttachObserver(IObserver* observerPtr) = 0;

	/**
		Detaches model object from observer \c observerPtr.
		\warning Please note, that  call of this function
		for the \c observer which is not attached to the observer \c observer will cause an assertion.
		To prevent this, you can use the isAttached() function.
	*/
	virtual void DetachObserver(IObserver* observerPtr) = 0;

	/**
		Detaches all attached observers.
	*/
	virtual void DetachAllObservers() = 0;

	/**
		Returns \c true if observer \c observer is attached to this model object.
	*/
	virtual bool IsAttached(const IObserver* observerPtr) const = 0;
};


} // namespace imod


#endif // !imod_IModel_included

