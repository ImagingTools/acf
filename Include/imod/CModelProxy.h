#ifndef imod_CModelProxy_included
#define imod_CModelProxy_included


// Qt includes
#include <QtCore/QVector>

// ACF includes
#include "imod/IModel.h"
#include "imod/TSingleModelObserverBase.h"


namespace imod
{


/**
	Implementation of the model proxy.
	This class can be used to switch model connections behind the proxy.

	\ingroup ModelObserver
*/
class CModelProxy: virtual public imod::IModel
{
public:
	CModelProxy();

	void SetModelPtr(imod::IModel* modelPtr);
	void ResetModel();

	// reimplemented (imod::IModel)
	virtual bool AttachObserver(IObserver* observerPtr);
	virtual void DetachObserver(IObserver* observerPtr);
	virtual void DetachAllObservers();
	virtual bool IsAttached(const IObserver* observerPtr) const;

private:
	void AttachProxyObservers();
	void DetachProxyObservers();

private:
	class ModelObserver: public imod::TSingleModelObserverBase<imod::IModel>
	{
	public:
		typedef imod::TSingleModelObserverBase<imod::IModel> BaseClass;

		ModelObserver(CModelProxy& parent);

		// reimplemented (imod::IObserver)
		virtual bool OnDetached(imod::IModel* modelPtr);

	private:
		CModelProxy& m_parent;
	};

	/**
		Observer and its pending state
	*/
	struct PendingObserver
	{
		PendingObserver(imod::IObserver* observerPtr = NULL, bool isPending = false)
			:m_observerPtr(observerPtr), 
			m_isPending(isPending)
		{
		}

		bool operator == (const imod::IObserver* observerPtr) const
		{
			return (observerPtr == m_observerPtr);
		}

		imod::IObserver* m_observerPtr;
		bool m_isPending;
	};

	typedef PendingObserver Observer;
	typedef QVector<Observer> Observers;

	Observers m_proxyObservers;
	ModelObserver m_modelObserver;
	imod::IModel* m_modelPtr;
};


} // namespace imod


#endif // !imod_CModelProxy_included


