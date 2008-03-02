#ifndef imod_IObserver_included
#define imod_IObserver_included


#include "imod/imod.h"


#include "istd/IPolymorphic.h"


namespace imod
{


class IModel;


/**
	\ingroup imod

	Common interface for all classe that implement the Observer functionality
	in the Model/Observer design pattern.
*/
class IObserver: virtual public istd::IPolymorphic
{
public:
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
	*/
	virtual void BeforeUpdate(IModel* modelPtr, int updateFlags = 0, istd::IPolymorphic* updateParamsPtr = NULL) = 0;

	/**
		This function will be called after update of the observer contents occures.
	*/
	virtual void AfterUpdate(IModel* modelPtr, int updateFlags = 0, istd::IPolymorphic* updateParamsPtr = NULL) = 0;
};


} // namespace imod


#endif // !imod_IObserver_included


