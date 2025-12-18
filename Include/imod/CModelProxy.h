#ifndef imod_CModelProxy_included
#define imod_CModelProxy_included


// Qt includes
#include <QtCore/QMutex>

// ACF includes
#include <imod/IModel.h>
#include <imod/TSingleModelObserverBase.h>


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
	virtual bool AttachObserver(IObserver* observerPtr) override;
	virtual void DetachObserver(IObserver* observerPtr) override;
	virtual void DetachAllObservers() override;
	virtual bool IsAttached(const IObserver* observerPtr) const override;

private:
	void AttachProxyObservers();
	void DetachProxyObservers();

private:
	class ModelObserver: public imod::TSingleModelObserverBase<imod::IModel>
	{
	public:
		typedef imod::TSingleModelObserverBase<imod::IModel> BaseClass;

		explicit ModelObserver(CModelProxy& parent);

		// reimplemented (imod::IObserver)
		virtual bool OnModelDetached(imod::IModel* modelPtr) override;

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
	typedef std::vector<Observer> Observers;

	Observers m_proxyObservers;
	ModelObserver m_modelObserver;
	imod::IModel* m_modelPtr;
#if QT_VERSION >= 0x060000
	mutable QRecursiveMutex m_modelMutex;
#else
	mutable QMutex m_modelMutex;
#endif
};


} // namespace imod


#endif // !imod_CModelProxy_included


