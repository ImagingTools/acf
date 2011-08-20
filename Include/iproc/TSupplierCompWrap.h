#ifndef iproc_TSupplierCompWrap_included
#define iproc_TSupplierCompWrap_included


// STL includes
#include <set>
#include <list>


// ACF includes
#include "istd/TChangeNotifier.h"
#include "istd/CStaticServicesProvider.h"

#include "imod/IModel.h"

#include "icomp/CComponentBase.h"

#include "ibase/TLoggerCompWrap.h"

#include "isys/ITimer.h"


// ACF includes
#include "iproc/ISupplier.h"


namespace iproc
{


/**
	Wrapper implementation of interface iproc::ISupplier with preparation for component implementation.
	During component initalization you should call \c AddInputSupplier for all suppliers used by this component as a input.
*/
template <class SupplierInterface, class Product>
class TSupplierCompWrap:
			public ibase::CLoggerComponentBase,
			virtual public SupplierInterface
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
		I_REGISTER_INTERFACE(SupplierInterface);
		I_ASSIGN(m_diagnosticNameAttrPtr, "DiagnosticName", "Name of this supplier for diagnostic, if it is not set, no diagnostic log message will be send", false, "");
		I_ASSIGN(m_paramsSetCompPtr, "ParamsSet", "Parameters set describing model parameter used to produce results", false, "ParamsSet");
		I_ASSIGN_TO(m_paramsSetModelCompPtr, m_paramsSetCompPtr, false);
	I_END_COMPONENT;

	TSupplierCompWrap();

	// pseudo-reimplemented (iproc::ISupplier)
	virtual void InvalidateSupplier();
	virtual void EnsureWorkFinished();
	virtual void ClearWorkResults();
	virtual int GetWorkStatus() const;
	virtual double GetWorkDurationTime() const;
	virtual iprm::IParamsSet* GetModelParametersSet() const;
	virtual void OnOutputSubscribed(ISupplier* outputSupplierPtr);
	virtual void OnOutputUnsubscribed(const ISupplier* outputSupplierPtr);

protected:
	/**
		Get current work product, if work was done correctly.
	*/
	const Product* GetWorkProduct() const;

	/**
		Add some supplier to input supplier list.
	*/
	void AddInputSupplier(ISupplier* supplierPtr);
	/**
		Remove supplier from input supplier list.
	*/
	void RemoveInputSupplier(ISupplier* supplierPtr);
	/**
		Remove all suppliers from input supplier list.
	*/
	void RemoveAllInputSuppliers();

	// abstract methods
	/**
		Produce single object.
		\return	work status. \sa iproc::ISupplier::WorkStatus
	*/
	virtual int ProduceObject(Product& result) const = 0;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

private:
	I_ATTR(istd::CString, m_diagnosticNameAttrPtr);
	I_REF(iprm::IParamsSet, m_paramsSetCompPtr);
	I_REF(imod::IModel, m_paramsSetModelCompPtr);

	typedef std::set<ISupplier*> Suppliers;
	Suppliers m_inputSuppliers;
	Suppliers m_outputSuppliers;

	Product m_product;
	int m_workStatus;
	double m_durationTime;
};


// public methods

template <class SupplierInterface, class Product>
TSupplierCompWrap<SupplierInterface, Product>::TSupplierCompWrap()
:	m_workStatus(WS_NONE),
	m_durationTime(0)
{
}


template <class SupplierInterface, class Product>
iprm::IParamsSet* TSupplierCompWrap<SupplierInterface, Product>::GetModelParametersSet() const
{
	return m_paramsSetCompPtr.GetPtr();
}


// pseudo-reimplemented (iproc::ISupplier)

template <class SupplierInterface, class Product>
void TSupplierCompWrap<SupplierInterface, Product>::InvalidateSupplier()
{
	if (m_workStatus == ISupplier::WS_LOCKED){
		return;
	}

	if (m_workStatus != ISupplier::WS_INIT){
		istd::CChangeNotifier notifier(this, ISupplier::CF_SUPPLIER_RESULTS);

		m_workStatus = ISupplier::WS_LOCKED;

		for (		Suppliers::const_iterator iter = m_outputSuppliers.begin();
					iter != m_outputSuppliers.end();
					++iter){
			ISupplier* supplierPtr = *iter;
			I_ASSERT(supplierPtr != NULL);

			supplierPtr->InvalidateSupplier();
		}

		m_workStatus = ISupplier::WS_INIT;
		m_durationTime = 0;
	}
}


template <class SupplierInterface, class Product>
void TSupplierCompWrap<SupplierInterface, Product>::EnsureWorkFinished()
{
	if (m_workStatus <= ISupplier::WS_INIT){
		istd::CChangeNotifier notifier(this, ISupplier::CF_SUPPLIER_RESULTS);

		m_workStatus = WS_LOCKED;

		istd::TSmartPtr<isys::ITimer> timerPtr(istd::CreateService<isys::ITimer>());

		if (timerPtr.IsValid()){
			// before time measurement is started, we have to ensure that all input suppliers has work finished
			for (		Suppliers::const_iterator iter = m_inputSuppliers.begin();
						iter != m_inputSuppliers.end();
						++iter){
				ISupplier* supplierPtr = *iter;
				I_ASSERT(supplierPtr != NULL);

				supplierPtr->EnsureWorkFinished();
			}

			timerPtr->Start();
		}

		m_workStatus = ProduceObject(m_product);
		I_ASSERT(m_workStatus >= WS_OK);	// No initial states are possible

		if (timerPtr.IsValid()){
			m_durationTime = timerPtr->GetElapsed();

			if (m_diagnosticNameAttrPtr.IsValid() && !(*m_diagnosticNameAttrPtr).empty()){
				SendInfoMessage(MI_DURATION_TIME, *m_diagnosticNameAttrPtr + ": Calculation time " + istd::CString::FromNumber(m_durationTime * 1000) + " ms.");
			}
		}
	}
}


template <class SupplierInterface, class Product>
void TSupplierCompWrap<SupplierInterface, Product>::ClearWorkResults()
{
	if (m_workStatus != ISupplier::WS_NONE){
		istd::CChangeNotifier notifier(this, ISupplier::CF_SUPPLIER_RESULTS);

		m_workStatus = ISupplier::WS_NONE;
		m_durationTime = 0;

		for (		Suppliers::const_iterator iter = m_inputSuppliers.begin();
					iter != m_inputSuppliers.end();
					++iter){
			ISupplier* supplierPtr = *iter;
			I_ASSERT(supplierPtr != NULL);

			supplierPtr->ClearWorkResults();
		}
	}
}


template <class SupplierInterface, class Product>
int TSupplierCompWrap<SupplierInterface, Product>::GetWorkStatus() const
{
	return m_workStatus;
}


template <class SupplierInterface, class Product>
double TSupplierCompWrap<SupplierInterface, Product>::GetWorkDurationTime() const
{
	return m_durationTime;
}


template <class SupplierInterface, class Product>
void TSupplierCompWrap<SupplierInterface, Product>::OnOutputSubscribed(ISupplier* outputSupplierPtr)
{
	I_ASSERT(outputSupplierPtr != NULL);

	m_outputSuppliers.insert(outputSupplierPtr);
}


template <class SupplierInterface, class Product>
void TSupplierCompWrap<SupplierInterface, Product>::OnOutputUnsubscribed(const ISupplier* outputSupplierPtr)
{
	I_ASSERT(outputSupplierPtr != NULL);
	I_ASSERT(m_outputSuppliers.find(const_cast<ISupplier*>(outputSupplierPtr)) != m_outputSuppliers.end());

	m_outputSuppliers.erase(const_cast<ISupplier*>(outputSupplierPtr));
}


// protected methods

template <class SupplierInterface, class Product>
const Product* TSupplierCompWrap<SupplierInterface, Product>::GetWorkProduct() const
{
	const_cast< TSupplierCompWrap<SupplierInterface, Product>* >(this)->EnsureWorkFinished();

	if (m_workStatus == WS_OK){
		return &m_product;
	}
	else{
		return NULL;
	}
}


template <class SupplierInterface, class Product>
void TSupplierCompWrap<SupplierInterface, Product>::AddInputSupplier(ISupplier* supplierPtr)
{
	I_ASSERT(supplierPtr != NULL);

	std::pair<Suppliers::iterator, bool> status = m_inputSuppliers.insert(supplierPtr);

	if (status.second){
		supplierPtr->OnOutputSubscribed(this);
	}
}


template <class SupplierInterface, class Product>
void TSupplierCompWrap<SupplierInterface, Product>::RemoveInputSupplier(ISupplier* supplierPtr)
{
	I_ASSERT(supplierPtr != NULL);

	supplierPtr->OnOutputUnsubscribed(this);

	m_inputSuppliers.erase(supplierPtr);
}


template <class SupplierInterface, class Product>
void TSupplierCompWrap<SupplierInterface, Product>::RemoveAllInputSuppliers()
{
	for (		Suppliers::const_iterator iter = m_inputSuppliers.begin();
				iter != m_inputSuppliers.end();
				++iter){
		ISupplier* supplierPtr = *iter;
		I_ASSERT(supplierPtr != NULL);

		supplierPtr->OnOutputUnsubscribed(this);
	}

	m_inputSuppliers.clear();
}


// reimplemented (icomp::CComponentBase)

template <class SupplierInterface, class Product>
void TSupplierCompWrap<SupplierInterface, Product>::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	m_workStatus = ISupplier::WS_NONE;
}


template <class SupplierInterface, class Product>
void TSupplierCompWrap<SupplierInterface, Product>::OnComponentDestroyed()
{
	RemoveAllInputSuppliers();

	BaseClass::OnComponentDestroyed();
}


} // namespace iproc


#endif // !iproc_TSupplierCompWrap_included


