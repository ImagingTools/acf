#ifndef imod_CModelBase_included
#define imod_CModelBase_included


// Qt includes
#include <QtCore/QMap>

// ACF includes
#include "imod/imod.h"
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
	typedef QSet<IObserver*> Observers;

	CModelBase();
	virtual ~CModelBase();

	/**
		Returns count of connected observers.
	*/
	virtual int GetObserverCount() const;

	/**
		Returns set of all observers.
	*/
	virtual Observers GetObservers() const;

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
	CModelBase(const CModelBase& modelBase);

	void CleanupObserverState();

private:
	/**
		Observer connection state.
	*/
	enum AttachingState
	{
		/**
			Observer is being to attached.
		*/
		AS_ATTACHING,

		/**
			Observer is connected to the model.
		*/
		AS_ATTACHED,
		AS_ATTACHED_UPDATING,
		/**
			Observer is in detaching stage.
		*/
		AS_DETACHING,

		/**
			Observer is in detached and connection will be removed.
		*/
		AS_DETACHED
	};

	typedef QMap<IObserver*, AttachingState> ObserversMap;
	ObserversMap m_observers;

	enum NotifyState
	{
		NS_NONE,
		NS_SENDING_BEFORE,
		NS_UPDATE,
		NS_SENDING_AFTER
	};
	NotifyState m_notifyState;
	int m_notifyFlags;
};


} // namespace imod


#endif // !imod_CModelBase_included

