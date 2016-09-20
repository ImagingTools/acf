#ifndef imod_IObserver_included
#define imod_IObserver_included


// ACF includes
#include <istd/IPolymorphic.h>
#include <istd/IChangeable.h>
#include <imod/imod.h>


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
	virtual bool IsModelAttached(const IModel* modelPtr = NULL) const = 0;
	/**
		This call back function will be called, if an observable object is about to be attached.
		This function returns a \c true, if observable object is accepted 
		by the implementation logic of the observer, otherwise returns false.
		\param	modelPtr	pointer to model object being attached.
		\param	changeMask	output parameter returning set of change ID's will be accepted.
		returns a \c false.
	*/
	virtual bool OnModelAttached(IModel* modelPtr, istd::IChangeable::ChangeSet& changeMask) = 0;

	/**
		This call back function will be called, if an observable object is about to be detached.
	*/
	virtual bool OnModelDetached(IModel* modelPtr) = 0;

	/**
		This function will be called before update of the observer contents occures.
		Please note, that in some cases no following AfterUpdate can be called.
	*/
	virtual void BeforeUpdate(IModel* modelPtr) = 0;

	/**
		This function will be called after update of the observer contents occures.
	*/
	virtual void AfterUpdate(IModel* modelPtr, const istd::IChangeable::ChangeSet& changeSet) = 0;
};


} // namespace imod


#endif // !imod_IObserver_included


