#ifndef iproc_TSupplierCompWrap_included
#define iproc_TSupplierCompWrap_included


// Qt includes
#include <QtCore/QSet>

// ACF includes
#include "istd/TChangeNotifier.h"

#include "imod/IModel.h"
#include "imod/CMultiModelObserverBase.h"

#include "icomp/CComponentBase.h"

#include "ibase/TLoggerCompWrap.h"


// ACF includes
#include "iproc/ISupplier.h"


namespace iproc
{


/**
	Wrapper implementation of interface iproc::ISupplier with preparation for component implementation.
	During component initalization you should call \c RegisterSupplierInput for all suppliers used by this component as a input.
*/
template <class Product>
class TSupplierCompWrap:
			public ibase::CLoggerComponentBase,
			virtual public ISupplier
{
public:
	typedef ibase::CLoggerComponentBase BaseClass;
	typedef Product ProductType;

	enum MessageId
	{
		MI_DURATION_TIME = 0x077a1b
	};

	I_BEGIN_BASE_COMPONENT(TSupplierCompWrap);
		I_REGISTER_INTERFACE(ISupplier);
		I_ASSIGN(m_diagnosticNameAttrPtr, "DiagnosticName", "Name of this supplier for diagnostic, if it is not set, no diagnostic log message will be send", false, "");
		I_ASSIGN(m_paramsSetCompPtr, "ParamsSet", "Parameters set describing model parameter used to produce results", false, "ParamsSet");
		I_ASSIGN_TO(m_paramsSetModelCompPtr, m_paramsSetCompPtr, false);
	I_END_COMPONENT;

	TSupplierCompWrap();

	// reimplemented (iproc::ISupplier)
	virtual void InvalidateSupplier();
	virtual void EnsureWorkFinished();
	virtual void ClearWorkResults();
	virtual int GetWorkStatus() const;
	virtual iprm::IParamsSet* GetModelParametersSet() const;

protected:
	/**
		Get current work product, if work was done correctly.
	*/
	const Product* GetWorkProduct() const;

	/**
		Register supplier input.
		Changes of supplier input will force this supplier invalidate.
		All registered inputs will be unregistered ducring component destroing (OnComponentDestryed method).
	*/
	virtual void RegisterSupplierInput(imod::IModel* modelPtr);
	/**
		Unregister supplier input.
		Changes of this input will no more invalidate this supplier.
	*/
	virtual void UnregisterSupplierInput(imod::IModel* modelPtr);

	// abstract methods
	/**
		Produce single object.
		\return	work status. \sa iproc::ISupplier::WorkStatus
	*/
	virtual int ProduceObject(Product& result) const = 0;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

	class InputObserver: public imod::CMultiModelObserverBase
	{
	public:
		InputObserver(TSupplierCompWrap<Product>* parentPtr);

		using imod::CMultiModelObserverBase::EnsureModelsDetached;

	protected:
		// reimplemented (imod::CMultiModelObserverBase)
		virtual void OnUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr);

	private:
		TSupplierCompWrap<Product>& m_parent;
	};

private:
	I_ATTR(QString, m_diagnosticNameAttrPtr);
	I_REF(iprm::IParamsSet, m_paramsSetCompPtr);
	I_REF(imod::IModel, m_paramsSetModelCompPtr);

	istd::TDelPtr<Product> m_productPtr;
	int m_workStatus;

	InputObserver m_inputObserver;
};


// public methods

template <class Product>
TSupplierCompWrap<Product>::TSupplierCompWrap()
:	m_workStatus(WS_NONE),
	m_inputObserver(this)
{
}


template <class Product>
iprm::IParamsSet* TSupplierCompWrap<Product>::GetModelParametersSet() const
{
	return m_paramsSetCompPtr.GetPtr();
}


// pseudo-reimplemented (iproc::ISupplier)

template <class Product>
void TSupplierCompWrap<Product>::InvalidateSupplier()
{
	if (m_workStatus == ISupplier::WS_LOCKED){
		return;
	}

	if (m_workStatus != ISupplier::WS_INIT){
		istd::CChangeNotifier notifier(this, ISupplier::CF_SUPPLIER_RESULTS);

		m_workStatus = ISupplier::WS_INIT;
	}
}


template <class Product>
void TSupplierCompWrap<Product>::EnsureWorkFinished()
{
	if (m_workStatus <= ISupplier::WS_INIT){
		istd::CChangeNotifier notifier(this, ISupplier::CF_SUPPLIER_RESULTS);

		m_workStatus = WS_LOCKED;

		if (!m_productPtr.IsValid()){
			m_productPtr.SetPtr(new Product());
		}

		m_workStatus = ProduceObject(*m_productPtr);
		I_ASSERT(m_workStatus >= WS_OK);	// No initial states are possible
	}
}


template <class Product>
void TSupplierCompWrap<Product>::ClearWorkResults()
{
	if (m_workStatus != ISupplier::WS_NONE){
		istd::CChangeNotifier notifier(this, ISupplier::CF_SUPPLIER_RESULTS);

		m_productPtr.Reset();

		m_workStatus = ISupplier::WS_NONE;
	}
}


template <class Product>
int TSupplierCompWrap<Product>::GetWorkStatus() const
{
	return m_workStatus;
}


// protected methods

template <class Product>
const Product* TSupplierCompWrap<Product>::GetWorkProduct() const
{
	const_cast< TSupplierCompWrap<Product>* >(this)->EnsureWorkFinished();

	if (m_workStatus == WS_OK){
		return m_productPtr.GetPtr();
	}
	else{
		return NULL;
	}
}


template <class Product>
void TSupplierCompWrap<Product>::RegisterSupplierInput(imod::IModel* modelPtr)
{
	I_ASSERT(modelPtr != NULL);
	modelPtr->AttachObserver(&m_inputObserver);
}


template <class Product>
void TSupplierCompWrap<Product>::UnregisterSupplierInput(imod::IModel* modelPtr)
{
	I_ASSERT(modelPtr != NULL);
	if (m_inputObserver.IsModelAttached(modelPtr)){
		modelPtr->DetachObserver(&m_inputObserver);
	}
}


// reimplemented (icomp::CComponentBase)

template <class Product>
void TSupplierCompWrap<Product>::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_paramsSetModelCompPtr.IsValid()){
		m_paramsSetModelCompPtr->AttachObserver(&m_inputObserver);
	}

	m_workStatus = ISupplier::WS_NONE;
}


template <class Product>
void TSupplierCompWrap<Product>::OnComponentDestroyed()
{
	m_inputObserver.EnsureModelsDetached();

	BaseClass::OnComponentDestroyed();
}


// public methods of embedded class InputObserver

template <class Product>
TSupplierCompWrap<Product>::InputObserver::InputObserver(TSupplierCompWrap<Product>* parentPtr)
:	m_parent(*parentPtr)
{
	I_ASSERT(parentPtr != NULL);
}


// protected methods of embedded class InputObserver

// reimplemented (imod::CMultiModelObserverBase)

template <class Product>
void TSupplierCompWrap<Product>::InputObserver::OnUpdate(imod::IModel* /*modelPtr*/, int /*updateFlags*/, istd::IPolymorphic* /*updateParamsPtr*/)
{
	m_parent.InvalidateSupplier();
}


} // namespace iproc


#endif // !iproc_TSupplierCompWrap_included


