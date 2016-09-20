#ifndef imod_CModelBase_included
#define imod_CModelBase_included


// Qt includes
#include <QtCore/QMap>

// ACF includes
#include <imod/imod.h>
#include <imod/IModel.h>
#include <imod/IObserver.h>


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

	const istd::IChangeable::ChangeSet& GetCumulatedChanges() const;

	// reimplemented (imod::IModel)
	virtual bool AttachObserver(imod::IObserver* observerPtr);
	virtual void DetachObserver(imod::IObserver* observerPtr);
	virtual void DetachAllObservers();
	virtual bool IsAttached(const imod::IObserver* observerPtr) const;

protected:
	/**
		Called before each change.
		\return	true, if some notification was done.
	*/
	void NotifyBeforeChange(const istd::IChangeable::ChangeSet& changeSet, bool isGroup);
	/**
		Called after each change.
		\return	true, if end notification was done.
	*/
	void NotifyAfterChange(const istd::IChangeable::ChangeSet& changeSet);

	/**
		Called before global changes will be started.
	*/
	virtual void OnBeginGlobalChanges() = 0;
	/**
		Called after global changes are finished.
	*/
	virtual void OnEndGlobalChanges(const istd::IChangeable::ChangeSet& changeSet) = 0;

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
		/**
			Observer is connected to the model and it is during the update.
		*/
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

	struct ObserverInfo
	{
		AttachingState state;
		istd::IChangeable::ChangeSet mask;
	};

	typedef QMap<IObserver*, ObserverInfo> ObserversMap;
	ObserversMap m_observers;

	int m_blockCounter;
	bool m_isDuringChanges;
	istd::IChangeable::ChangeSet m_cumulatedChangeIds;
};


// public inline methods

inline const istd::IChangeable::ChangeSet& CModelBase::GetCumulatedChanges() const
{
	return m_cumulatedChangeIds;
}


} // namespace imod


#endif // !imod_CModelBase_included

