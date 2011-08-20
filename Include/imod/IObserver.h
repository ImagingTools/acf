#ifndef imod_IObserver_included
#define imod_IObserver_included


// ACF includes
#include "istd/IPolymorphic.h"

#include "imod/imod.h"


namespace imod
{


class IModel;


/**
	Common interface for all classes implementing the Observer functionality
	in the Model/Observer design pattern.

	\ingroup ModelObserver
*/
class IObserver: virtual public istd::IPolymorphic
{
public:
	/**
		Check if specified model is attached.
		\modelPtr	pointer to model object. If this pointer is NULL, any model is meaned.
		\return	true, if \c modelPtr is attached yet to this observer, false otherwise.
	*/
	virtual bool IsModelAttached(const IModel* modelPtr) const = 0;
	/**
		This call back function will be called, if an observable object is about to be attached.
		This function returns a \c true, if observable object is accepted 
		by the implementation logic of the observer, otherwise the function 
		returns a \c false.
	*/
	virtual bool OnAttached(IModel* modelPtr) = 0;

	/**
		This call back function will be called, if an observable object is about to be detached.
	*/
	virtual bool OnDetached(IModel* modelPtr) = 0;

	/**
		This function will be called before update of the observer contents occures.
		Please note, that in some cases no following AfterUpdate can be called.
	*/
	virtual void BeforeUpdate(IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr) = 0;

	/**
		This function will be called after update of the observer contents occures.
	*/
	virtual void AfterUpdate(IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr) = 0;
};


} // namespace imod


#endif // !imod_IObserver_included


