#ifndef imod_CMultiModelObserverBase_included
#define imod_CMultiModelObserverBase_included


// STL includes
#include <vector>

// ACF includes
#include "imod/IModel.h"
#include "imod/IObserver.h"


namespace imod
{


/**
	Basic implementation of a multiple model observer.
*/
class CMultiModelObserverBase: virtual public IObserver
{
public:
	virtual ~CMultiModelObserverBase();

	/**
		Get access to connected model with the index \c index.
	*/
	IModel* GetModelPtr(int modelIndex) const;

	/**
		Gets the number of connected models.	
	*/
	int GetModelCount() const;

	// reimplemented (imod::IObserver)
	virtual bool IsModelAttached(const IModel* modelPtr) const;
	virtual bool OnAttached(IModel* modelPtr);
	virtual bool OnDetached(IModel* modelPtr);
	virtual void BeforeUpdate(IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr);
	virtual void AfterUpdate(IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr);

protected:
	void EnsureModelsDetached();

	virtual void OnUpdate(IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr);

private:
	typedef std::vector<IModel*> Models;

	Models m_models;
};


} // namespace imod


#endif // !imod_CMultiModelObserverBase_included

