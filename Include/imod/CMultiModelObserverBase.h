#ifndef imod_CMultiModelObserverBase_included
#define imod_CMultiModelObserverBase_included


// STL includes
#include <vector>

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
	imod::IModel* GetModelPtr(int modelIndex) const;

	/**
		Gets the number of connected models.	
	*/
	int GetModelCount() const;

	// reimplemented (imod::IObserver)
	virtual bool IsModelAttached(const imod::IModel* modelPtr) const;
	virtual bool OnAttached(imod::IModel* modelPtr);
	virtual bool OnDetached(imod::IModel* modelPtr);
	virtual void BeforeUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr);
	virtual void AfterUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr);

protected:
	void EnsureDetached();

	virtual void OnUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr);

private:
	typedef std::vector<imod::IModel*> Models;

	Models m_models;
};


} // namespace imod


#endif // !imod_CMultiModelObserverBase_included

