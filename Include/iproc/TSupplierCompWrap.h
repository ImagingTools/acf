#ifndef iproc_TSupplierCompWrap_included
#define iproc_TSupplierCompWrap_included


// STL includes
#include <map>
#include <list>


// ACF includes
#include "istd/TChangeNotifier.h"
#include "istd/CStaticServicesProvider.h"

#include "imod/IModel.h"
#include "imod/CSingleModelObserverBase.h"

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
			private imod::CSingleModelObserverBase,
			virtual public SupplierInterface
{
public:
	typedef ibase::CLoggerComponentBase BaseClass;
	typedef imod::CSingleModelObserverBase BaseClass2;

	I_BEGIN_BASE_COMPONENT(TSupplierCompWrap);
		I_REGISTER_INTERFACE(iser::ISerializable);
		I_REGISTER_INTERFACE(ISupplier);
		I_REGISTER_INTERFACE(SupplierInterface);
		I_ASSIGN(m_paramsSetCompPtr, "ParamsSet", "Parameters set describing model parameter used to produce results", false, "ParamsSet");
		I_ASSIGN(m_paramsSetModelCompPtr, "ParamsSet", "Parameters set describing model parameter used to produce results", false, "ParamsSet");
	I_END_COMPONENT;

	// pseudo-reimplemented (iproc::ISupplier)
	virtual void InitNewWork();
	virtual void EnsureWorkFinished();
	virtual void ClearWorkResults();
	virtual int GetWorkStatus() const;
	virtual double GetWorkDurationTime() const;
	virtual iprm::IParamsSet* GetModelParametersSet() const;

	// pseudo-reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);
	virtual I_DWORD GetMinimalVersion(int versionId = iser::IVersionInfo::UserVersionId) const;

	// reimplemented (icomp::IComponent)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

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
	void RemoveInputSupplier(const ISupplier* supplierPtr);
	/**
		Remove all suppliers from input supplier list.
	*/
	void RemoveAllInputSuppliers();

	// reimplemented (imod::CSingleModelObserverBase)
	virtual void OnUpdate(int updateFlags, istd::IPolymorphic* updateParamsPtr);

	// pseudo-reimplemented (istd::IChangeable)
	virtual void OnEndChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr);

	// abstract methods
	/**
		Produce single object.
		\return	work status. \sa iproc::ISupplier::WorkStatus
	*/
	virtual int ProduceObject(Product& result) const = 0;

private:
	I_ATTR(int, m_recentObjectListSizeAttrPtr);
	I_REF(iprm::IParamsSet, m_paramsSetCompPtr);
	I_REF(imod::IModel, m_paramsSetModelCompPtr);

	typedef std::set<ISupplier*> InputSuppliers;
	InputSuppliers m_inputSuppliers;

	Product m_product;
	int m_workStatus;
	double m_durationTime;
};


// public methods

template <class SupplierInterface, class Product>
iprm::IParamsSet* TSupplierCompWrap<SupplierInterface, Product>::GetModelParametersSet() const
{
	return m_paramsSetCompPtr.GetPtr();
}


// pseudo-reimplemented (iproc::ISupplier)

template <class SupplierInterface, class Product>
void TSupplierCompWrap<SupplierInterface, Product>::InitNewWork()
{
	if (m_workStatus != ISupplier::WS_INIT){
		m_workStatus = ISupplier::WS_INIT;
		m_durationTime = 0;

		for (		InputSuppliers::const_iterator iter = m_inputSuppliers.begin();
					iter != m_inputSuppliers.end();
					++iter){
			ISupplier* supplierPtr = *iter;
			if (supplierPtr != NULL){
				supplierPtr->InitNewWork();
			}
		}
	}
}


template <class SupplierInterface, class Product>
void TSupplierCompWrap<SupplierInterface, Product>::EnsureWorkFinished()
{
	if (m_workStatus <= ISupplier::WS_INIT){
		m_workStatus = ISupplier::WS_LOCKED;

		isys::ITimer* timerPtr = istd::GetService<isys::ITimer>();

		double beforeTime = 0;
		if (timerPtr != NULL){
			beforeTime = timerPtr->GetElapsed();
		}

		istd::CChangeNotifier notifier(this, ISupplier::CF_SUPPLIER_RESULTS);

		m_workStatus = ProduceObject(m_product);
		I_ASSERT(m_workStatus >= WS_OK);	// No initial states are possible

		if (timerPtr != NULL){
			m_durationTime = timerPtr->GetElapsed() - beforeTime;
		}
	}
}


template <class SupplierInterface, class Product>
void TSupplierCompWrap<SupplierInterface, Product>::ClearWorkResults()
{
	if (m_workStatus != ISupplier::WS_NONE){
		m_workStatus = ISupplier::WS_NONE;
		m_durationTime = 0;

		for (		InputSuppliers::const_iterator iter = m_inputSuppliers.begin();
					iter != m_inputSuppliers.end();
					++iter){
			ISupplier* supplierPtr = *iter;
			if (supplierPtr != NULL){
				supplierPtr->ClearWorkResults();
			}
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


// reimplemented (iser::ISerializable)

template <class SupplierInterface, class Product>
bool TSupplierCompWrap<SupplierInterface, Product>::Serialize(iser::IArchive& archive)
{
	if (m_paramsSetCompPtr.IsValid()){
		return m_paramsSetCompPtr->Serialize(archive);
	}

	return true;
}


template <class SupplierInterface, class Product>
I_DWORD TSupplierCompWrap<SupplierInterface, Product>::GetMinimalVersion(int versionId) const
{
	if (m_paramsSetCompPtr.IsValid()){
		return m_paramsSetCompPtr->GetMinimalVersion(versionId);
	}

	return SupplierInterface::GetMinimalVersion(versionId);
}


// reimplemented (icomp::IComponent)

template <class SupplierInterface, class Product>
void TSupplierCompWrap<SupplierInterface, Product>::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_paramsSetModelCompPtr.IsValid()){
		m_paramsSetModelCompPtr->AttachObserver(this);
	}
}


template <class SupplierInterface, class Product>
void TSupplierCompWrap<SupplierInterface, Product>::OnComponentDestroyed()
{
	if (m_paramsSetModelCompPtr.IsValid()){
		m_paramsSetModelCompPtr->DetachObserver(this);
	}

	RemoveAllInputSuppliers();

	BaseClass::OnComponentDestroyed();
}


// protected methods

template <class SupplierInterface, class Product>
const Product* TSupplierCompWrap<SupplierInterface, Product>::GetWorkProduct() const
{
	const_cast< TSupplierCompWrap<SupplierInterface, Product>* >(this)->EnsureWorkFinished();

	if ((m_workStatus == WS_OK) || (m_workStatus == WS_WARNING)){
		return &m_product;
	}
	else{
		return NULL;
	}
}


// reimplemented (imod::CSingleModelObserverBase)

template <class SupplierInterface, class Product>
void TSupplierCompWrap<SupplierInterface, Product>::AddInputSupplier(ISupplier* supplierPtr)
{
	m_inputSuppliers.insert(supplierPtr);
}


template <class SupplierInterface, class Product>
void TSupplierCompWrap<SupplierInterface, Product>::RemoveInputSupplier(const ISupplier* supplierPtr)
{
	m_inputSuppliers.erase(supplierPtr);
}


template <class SupplierInterface, class Product>
void TSupplierCompWrap<SupplierInterface, Product>::RemoveAllInputSuppliers()
{
	m_inputSuppliers.clear();
}


template <class SupplierInterface, class Product>
void TSupplierCompWrap<SupplierInterface, Product>::OnUpdate(int updateFlags, istd::IPolymorphic* updateParamsPtr)
{
	m_workStatus = ISupplier::WS_NONE;

	BaseClass2::OnUpdate(updateFlags, updateParamsPtr);
}


// pseudo-reimplemented (istd::IChangeable)

template <class SupplierInterface, class Product>
void TSupplierCompWrap<SupplierInterface, Product>::OnEndChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr)
{
	if ((changeFlags & istd::IChangeable::CF_MODEL) != 0){
		m_workStatus = ISupplier::WS_NONE;
	}

	SupplierInterface::OnEndChanges(changeFlags, changeParamsPtr);
}


} // namespace iproc


#endif // !iproc_TSupplierCompWrap_included


