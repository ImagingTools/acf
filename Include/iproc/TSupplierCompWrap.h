#ifndef iproc_TSupplierCompWrap_included
#define iproc_TSupplierCompWrap_included


// Qt includes
#include <QtCore/QSet>
#include <QtCore/QElapsedTimer>

// ACF includes
#include "istd/IChangeable.h"
#include "istd/TChangeNotifier.h"

#include "imod/IModel.h"
#include "imod/CMultiModelObserverBase.h"

#include "icomp/CComponentBase.h"

#include "ibase/TLoggerCompWrap.h"


// ACF includes
#include "iproc/ISupplier.h"
#include "iproc/IElapsedTimeProvider.h"


namespace iproc
{


/**
	Wrapper implementation of interface iproc::ISupplier with preparation for component implementation.
	During component initalization you should call \c RegisterSupplierInput for all suppliers used by this component as a input.
*/
template <class Product>
class TSupplierCompWrap:
			public ibase::CLoggerComponentBase,
			virtual public ISupplier,
			virtual public IElapsedTimeProvider,
			virtual public istd::IChangeable
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
		I_REGISTER_INTERFACE(IElapsedTimeProvider);
		I_ASSIGN(m_diagnosticNameAttrPtr, "DiagnosticName", "Name of this supplier for diagnostic, if it is not set, no diagnostic log message will be send", false, "");
		I_ASSIGN(m_paramsSetCompPtr, "ParamsSet", "Parameters set describing model parameter used to produce results", false, "ParamsSet");
		I_ASSIGN_TO(m_paramsSetModelCompPtr, m_paramsSetCompPtr, false);
		I_ASSIGN_MULTI_0(m_additionalTriggerInputsCompPtr, "AdditionalTriggerInputs", "Additional observed trigger inputs, the supplier will be invalidated if some input signal the change", false);
	I_END_COMPONENT;

	TSupplierCompWrap();

	// reimplemented (iproc::ISupplier)
	virtual void InvalidateSupplier();
	virtual void EnsureWorkInitialized();
	virtual void EnsureWorkFinished();
	virtual void ClearWorkResults();
	virtual int GetWorkStatus() const;
	virtual iprm::IParamsSet* GetModelParametersSet() const;

	// reimplemented (iproc::IElapsedTimeProvider)
	virtual double GetElapsedTime() const;

protected:
	/**
		Called if the new work should be initiliazed.
		Default implementation do nothing. It is dedicated to be overriden.
	*/
	virtual bool InitializeWork();

	/**
		Called if the supplier parameters was changed.
		Default implementation do nothing. It is dedicated to be overriden.
	*/
	virtual void OnParametersChanged();

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

	class InputsObserver: public imod::CMultiModelObserverBase
	{
	public:
		InputsObserver(TSupplierCompWrap<Product>* parentPtr);

		using imod::CMultiModelObserverBase::EnsureModelsDetached;

	protected:
		// reimplemented (imod::CMultiModelObserverBase)
		virtual void BeforeUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr);

		TSupplierCompWrap<Product>& m_parent;
	};

	class ParamsObserver: public imod::CMultiModelObserverBase
	{
	public:
		ParamsObserver(TSupplierCompWrap<Product>* parentPtr);

		using imod::CMultiModelObserverBase::EnsureModelsDetached;

	protected:
		// reimplemented (imod::CMultiModelObserverBase)
		virtual void AfterUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr);

		TSupplierCompWrap<Product>& m_parent;
	};

private:
	I_ATTR(QString, m_diagnosticNameAttrPtr);
	I_REF(iprm::IParamsSet, m_paramsSetCompPtr);
	I_REF(imod::IModel, m_paramsSetModelCompPtr);
	I_MULTIREF(imod::IModel, m_additionalTriggerInputsCompPtr);

	istd::TDelPtr<Product> m_productPtr;
	int m_workStatus;

	InputsObserver m_inputsObserver;
	ParamsObserver m_paramsObserver;

	istd::CChangeNotifier m_productChangeNotifier;

	double m_elapsedTime;

	bool m_areParametersValid;
};


// public methods

template <class Product>
TSupplierCompWrap<Product>::TSupplierCompWrap()
:	m_workStatus(WS_INVALID),
	m_inputsObserver(this),
	m_paramsObserver(this),
	m_productChangeNotifier(NULL, CF_SUPPLIER_RESULTS | CF_MODEL),
	m_elapsedTime(0),
	m_areParametersValid(false)
{
}


// pseudo-reimplemented (iproc::ISupplier)

template <class Product>
void TSupplierCompWrap<Product>::InvalidateSupplier()
{
	if (m_workStatus == ISupplier::WS_LOCKED){
		return;
	}

	m_elapsedTime = 0.0;

	if (m_workStatus != ISupplier::WS_INVALID){
		m_productChangeNotifier.SetPtr(this);

		m_workStatus = ISupplier::WS_INVALID;
	}
}


template <class Product>
void TSupplierCompWrap<Product>::EnsureWorkInitialized()
{
	if (m_workStatus < ISupplier::WS_INIT){
		m_productChangeNotifier.SetPtr(this);

		if (!m_areParametersValid){
			m_areParametersValid = true;

			OnParametersChanged();
		}

		if (InitializeWork()){
			m_workStatus = WS_INIT;
		}
		else{
			m_workStatus = WS_CRITICAL;

			m_productChangeNotifier.SetPtr(NULL);
		}
	}
}


template <class Product>
void TSupplierCompWrap<Product>::EnsureWorkFinished()
{
	if (m_workStatus <= ISupplier::WS_INIT){
		TSupplierCompWrap<Product>::EnsureWorkInitialized();

		m_workStatus = WS_LOCKED;

		if (!m_productPtr.IsValid()){
			m_productPtr.SetPtr(new Product());
		}

		QElapsedTimer timer;
		timer.start();

		m_workStatus = ProduceObject(*m_productPtr);
		I_ASSERT(m_workStatus >= WS_OK);	// No initial states are possible

		m_elapsedTime = timer.nsecsElapsed() / 1000000000.0;

		m_productChangeNotifier.SetPtr(NULL);
	}
}


template <class Product>
void TSupplierCompWrap<Product>::ClearWorkResults()
{
	if (m_workStatus == ISupplier::WS_LOCKED){
		return;
	}

	m_elapsedTime = 0.0;

	if (m_workStatus != ISupplier::WS_INVALID){
		m_productChangeNotifier.SetPtr(this);

		m_productPtr.Reset();

		m_workStatus = ISupplier::WS_INVALID;
	}
}


template <class Product>
int TSupplierCompWrap<Product>::GetWorkStatus() const
{
	return m_workStatus;
}


template <class Product>
iprm::IParamsSet* TSupplierCompWrap<Product>::GetModelParametersSet() const
{
	return m_paramsSetCompPtr.GetPtr();
}


// reimplemented (iproc::IElapsedTimeProvider)

template <class Product>
double TSupplierCompWrap<Product>::GetElapsedTime() const
{
	return m_elapsedTime;
}


// protected methods

template <class Product>
bool TSupplierCompWrap<Product>::InitializeWork()
{
	return true;
}


template <class Product>
void TSupplierCompWrap<Product>::OnParametersChanged()
{
}


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
	modelPtr->AttachObserver(&m_inputsObserver);
}


template <class Product>
void TSupplierCompWrap<Product>::UnregisterSupplierInput(imod::IModel* modelPtr)
{
	I_ASSERT(modelPtr != NULL);
	if (m_inputsObserver.IsModelAttached(modelPtr)){
		modelPtr->DetachObserver(&m_inputsObserver);
	}
}


// reimplemented (icomp::CComponentBase)

template <class Product>
void TSupplierCompWrap<Product>::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_paramsSetModelCompPtr.IsValid()){
		m_paramsSetModelCompPtr->AttachObserver(&m_paramsObserver);
	}

	int inputsCount = m_additionalTriggerInputsCompPtr.GetCount();
	for (int i = 0; i < inputsCount; ++i){
		imod::IModel* inputModelPtr = m_additionalTriggerInputsCompPtr[i];
		if (inputModelPtr != NULL){
			RegisterSupplierInput(inputModelPtr);
		}
	}

	m_workStatus = ISupplier::WS_INVALID;
}


template <class Product>
void TSupplierCompWrap<Product>::OnComponentDestroyed()
{
	m_inputsObserver.EnsureModelsDetached();
	m_paramsObserver.EnsureModelsDetached();

	BaseClass::OnComponentDestroyed();
}


// public methods of embedded class InputsObserver

template <class Product>
TSupplierCompWrap<Product>::InputsObserver::InputsObserver(TSupplierCompWrap<Product>* parentPtr)
:	m_parent(*parentPtr)
{
	I_ASSERT(parentPtr != NULL);
}


// protected methods of embedded class InputsObserver

// reimplemented (imod::CMultiModelObserverBase)

template <class Product>
void TSupplierCompWrap<Product>::InputsObserver::BeforeUpdate(imod::IModel* /*modelPtr*/, int /*updateFlags*/, istd::IPolymorphic* /*updateParamsPtr*/)
{
	m_parent.InvalidateSupplier();
}


// public methods of embedded class ParamsObserver

template <class Product>
TSupplierCompWrap<Product>::ParamsObserver::ParamsObserver(TSupplierCompWrap<Product>* parentPtr)
:	m_parent(*parentPtr)
{
	I_ASSERT(parentPtr != NULL);
}


// protected methods of embedded class ParamsObserver

// reimplemented (imod::CMultiModelObserverBase)

template <class Product>
void TSupplierCompWrap<Product>::ParamsObserver::AfterUpdate(imod::IModel* /*modelPtr*/, int /*updateFlags*/, istd::IPolymorphic* /*updateParamsPtr*/)
{
	m_parent.m_areParametersValid = false;

	m_parent.InvalidateSupplier();
}


} // namespace iproc


#endif // !iproc_TSupplierCompWrap_included


