#ifndef imod_CMultiModelObserverBase_included
#define imod_CMultiModelObserverBase_included


// Qt includes
#include <QtCore/QVector>
#include <QtCore/QMutex>

// ACF includes
#include <imod/IModel.h>
#include <imod/IObserver.h>


namespace imod
{


/**
	Basic implementation of a multiple model observer.

	\ingroup ModelObserver
*/
class CMultiModelObserverBase: virtual public IObserver
{
public:
	CMultiModelObserverBase();
	virtual ~CMultiModelObserverBase();

	/**
		Gets the number of connected models.	
	*/
	int GetModelCount() const;

	/**
		Get access to connected model with the index \c index.
		\param	modelIndex index of model.
	*/
	IModel* GetObservedModel(int modelIndex) const;

	/**
		Get access to connected model with the index \c index.
		\deprecated use \c GetObservedModel instead.
	*/
	IModel* GetModelPtr(int modelIndex) const;

	/**
		Ensure all attached models are detached.
		If there are some attached models they will be detached and removed from observed list.
	*/
	void EnsureModelsDetached();

	/**
		Set list of ID's beeing observed.
	*/
	void SetObservedIds(const istd::IChangeable::ChangeSet& changeMask);

	// reimplemented (imod::IObserver)
	virtual bool IsModelAttached(const IModel* modelPtr) const;
	virtual bool OnModelAttached(IModel* modelPtr, istd::IChangeable::ChangeSet& changeMask);
	virtual bool OnModelDetached(IModel* modelPtr);
	virtual void BeforeUpdate(IModel* modelPtr);
	virtual void AfterUpdate(IModel* modelPtr, const istd::IChangeable::ChangeSet& changeSet);

protected:
	/**
		Called on update of observed model.
		This method is designed to be overload by derrived classes.
	*/
	virtual void OnUpdate(IModel* modelPtr, const istd::IChangeable::ChangeSet& changeSet);

private:
	bool IsAttached(const imod::IModel* modelPtr) const;

	typedef QVector<IModel*> Models;
	Models m_models;

	istd::IChangeable::ChangeSet m_observedIds;

	mutable QMutex m_mutex;
};


} // namespace imod


#endif // !imod_CMultiModelObserverBase_included

