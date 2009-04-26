#ifndef imod_CModelBase_included
#define imod_CModelBase_included


#include "imod/imod.h"


#include <vector>


#include "imod/IModel.h"
#include "imod/IObserver.h"


namespace imod
{		


/**
	Basic implementation of a model.

	\ingroup ModelObserver
*/
class CModelBase: virtual public imod::IModel
{
public:
	CModelBase();
	virtual ~CModelBase();

	/**
		Returns count of connected observers.
	*/
	virtual int GetObserverCount() const;

	/**
		Returns pointer to a given observer.
	*/
	virtual IObserver* GetObserverPtr(int index) const;

	// reimplemented (imod::IModel)
	virtual bool AttachObserver(imod::IObserver* observerPtr);
	virtual void DetachObserver(imod::IObserver* observerPtr);
	virtual void DetachAllObservers();
	virtual bool IsAttached(const imod::IObserver* observerPtr) const;

protected:
	/**
		Notifies all observers about begin of changes in the data model.
		For updating control you can use some specified flags \c updateFlags
		and for additional data transfer \c udpateParamsPtr.
		\sa NotifyAfterUpdate()
	*/
	virtual void NotifyBeforeUpdate(int updateFlags = 0, istd::IPolymorphic* updateParamsPtr = NULL);

	/**
		Notifies all observers that the changes are finished.
		For updating control you can use some specified flags \c updateFlags
		and for additional data transfer \c udpateParamsPtr.
		\sa NotifyBeforeUpdate()
	*/
	virtual void NotifyAfterUpdate(int updateFlags = 0, istd::IPolymorphic* updateParamsPtr = NULL);

private:
	CModelBase(const CModelBase&);

	typedef std::vector<IObserver*> Observers;
	Observers m_observers;
};


} // namespace imod


#endif // !imod_CModelBase_included

