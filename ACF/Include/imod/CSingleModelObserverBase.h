#ifndef imod_CSingleModelObserverBase_included
#define imod_CSingleModelObserverBase_included


#include "imod/IObserver.h" 


namespace imod
{


class IModel;


/**
	\ingroup imod

	Basic implementation for a single model observer.
*/
class CSingleModelObserverBase: virtual public IObserver 
{
public:
	CSingleModelObserverBase();
	~CSingleModelObserverBase();

	/**
		Get access to observed model.
		\return	pointer to observed model, or NULL if no model is connected.
	*/
	imod::IModel* GetModelPtr() const;

	// reimplemented (imod::IObserver)
	virtual bool OnAttached(imod::IModel* modelPtr);
	virtual bool OnDetached(imod::IModel* modelPtr);
	virtual void BeforeUpdate(imod::IModel* modelPtr, int updateFlags = 0, istd::IPolymorphic* updateParamsPtr = NULL);
	virtual void AfterUpdate(imod::IModel* modelPtr, int updateFlags = 0, istd::IPolymorphic* updateParamsPtr = NULL);

protected:
	/**
		Set internal model pointer to specified value.
	*/
	void SetModelPtr(imod::IModel* modelPtr);
	/**
		Make sure this observer is detached.
		It calls model method DetachObserver() if model is available.
	*/
	void EnsureDetached();

	// abstract methods
	virtual void OnUpdate(imod::IModel* modelPtr, int updateFlags = 0, istd::IPolymorphic* updateParamsPtr = NULL) = 0;

private:
	imod::IModel* m_modelPtr;
};


// inline methods

inline imod::IModel* CSingleModelObserverBase::GetModelPtr() const
{
	return m_modelPtr;
}


// protected inline methods

inline void CSingleModelObserverBase::SetModelPtr(imod::IModel* modelPtr)
{
	m_modelPtr = modelPtr;
}


} // namespace imod


#endif // !imod_CSingleModelObserverBase_included


