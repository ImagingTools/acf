#ifndef imod_CMultiModelBridgeBase_included
#define imod_CMultiModelBridgeBase_included


// Qt includes
#include <QtCore/QVector>

// ACF includes
#include "istd/IChangeable.h"

#include "imod/IModel.h"
#include "imod/IObserver.h"


namespace imod
{


/**
	Allow to connect as observer to multiple models and call BeginChanges and EndChanges on some its changes.
*/
class CMultiModelBridgeBase: virtual public IObserver, virtual public istd::IChangeable
{
public:
	virtual ~CMultiModelBridgeBase();

	/**
		Get access to connected model with the index \c index.
	*/
	imod::IModel* GetModelPtr(int modelIndex) const;

	/**
		Gets the number of connected models.	
	*/
	int GetModelCount() const;

	/**
		Remove all observed models from this bridge.
	*/
	void EnsureModelsDetached();

	// reimplemented (imod::IObserver)
	virtual bool IsModelAttached(const imod::IModel* modelPtr) const;
	virtual bool OnAttached(imod::IModel* modelPtr);
	virtual bool OnDetached(imod::IModel* modelPtr);
	virtual void BeforeUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr);
	virtual void AfterUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr);

private:
	typedef QVector<imod::IModel*> Models;

	Models m_models;
};


} // namespace imod


#endif // !imod_CMultiModelBridgeBase_included

