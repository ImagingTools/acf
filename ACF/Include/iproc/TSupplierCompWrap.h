#ifndef iproc_TSupplierCompWrap_included
#define iproc_TSupplierCompWrap_included


// STL includes
#include <map>
#include <list>

#include "istd/TChangeNotifier.h"
#include "istd/CStaticServicesProvider.h"

#include "imod/IModel.h"
#include "imod/CSingleModelObserverBase.h"

#include "icomp/CComponentBase.h"

#include "isys/ITimer.h"


namespace iproc
{


/**
	Wrapper implementation of interface iproc::ISupplier with preparation for component implementation.
*/
template <class SupplierInterface, class Product>
class TSupplierCompWrap:
			public icomp::CComponentBase,
			private imod::CSingleModelObserverBase,
			virtual public SupplierInterface
{
public:
	typedef icomp::CComponentBase BaseClass;
	typedef imod::CSingleModelObserverBase BaseClass2;

	I_BEGIN_BASE_COMPONENT(TSupplierCompWrap);
		I_REGISTER_INTERFACE(iser::ISerializable);
		I_REGISTER_INTERFACE(ISupplier);
		I_REGISTER_INTERFACE(SupplierInterface);
		I_ASSIGN(m_recentObjectListSizeAttrPtr, "RecentObjectListSize", "Size of list storing recent processed tagged objects, if it is disabled only one object will stored", false, 10);
		I_ASSIGN(m_paramsSetCompPtr, "ParamsSet", "Parameters set describing model parameter used to produce results", false, "ParamsSet");
		I_ASSIGN(m_paramsSetModelCompPtr, "ParamsSet", "Parameters set describing model parameter used to produce results", false, "ParamsSet");
	I_END_COMPONENT;

	// pseudo-reimplemented (iproc::ISupplier)
	virtual void BeginNextObject(I_DWORD objectId);
	virtual void EnsureWorkFinished(I_DWORD objectId);
	virtual int GetWorkStatus(I_DWORD objectId) const;
	virtual double GetWorkDurationTime(I_DWORD objectId) const;
	virtual iprm::IParamsSet* GetModelParametersSet() const;

	// pseudo-reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);
	virtual I_DWORD GetMinimalVersion(int versionId = iser::IVersionInfo::UserVersionId) const;

	// reimplemented (icomp::IComponent)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

protected:
	struct WorkInfo
	{
		Product product;
		int status;
		double durationTime;
		bool isDone;
	};

	/**
		Check if object ID is beeing processed or processing is started yet.
	*/
	bool IsIdKnown(I_DWORD objectId) const;

	/**
		Get complete product optional using object ID.
		\return	pointer to product instance if product is accessible, or NULL.
	*/
	const WorkInfo* GetWorkInfo(I_DWORD objectId, bool ensureFinished) const;

	/**
		Remove all cached and produced objects.
	*/
	void ResetProducedObjects();

	// reimplemented (imod::CSingleModelObserverBase)
	virtual void OnUpdate(int updateFlags, istd::IPolymorphic* updateParamsPtr);

	// pseudo-reimplemented (istd::IChangeable)
	virtual void OnEndChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr);

	// abstract methods
	/**
		Produce single object.
		\return	work status. \sa iproc::ISupplier::WorkStatus
	*/
	virtual int ProduceObject(I_DWORD objectId, Product& result) const = 0;

private:
	typedef std::map<I_DWORD, WorkInfo> StoredInfoMap;
	typedef std::list<I_DWORD> RecentIdList;

	mutable StoredInfoMap m_storedInfoMap;
	mutable RecentIdList m_recentIdList;

	I_ATTR(int, m_recentObjectListSizeAttrPtr);
	I_REF(iprm::IParamsSet, m_paramsSetCompPtr);
	I_REF(imod::IModel, m_paramsSetModelCompPtr);
};


// public methods

template <class SupplierInterface, class Product>
iprm::IParamsSet* TSupplierCompWrap<SupplierInterface, Product>::GetModelParametersSet() const
{
	return m_paramsSetCompPtr.GetPtr();
}


// pseudo-reimplemented (iproc::ISupplier)

template <class SupplierInterface, class Product>
void TSupplierCompWrap<SupplierInterface, Product>::BeginNextObject(I_DWORD objectId)
{
	StoredInfoMap::const_iterator foundIter = m_storedInfoMap.find(objectId);
	if (foundIter == m_storedInfoMap.end()){
		m_recentIdList.push_back(objectId);
		WorkInfo& workInfo = m_storedInfoMap[objectId];

		workInfo.status = WS_NONE;
		workInfo.durationTime = -1;
		workInfo.isDone = false;
	}

	I_ASSERT(m_storedInfoMap.size() == m_recentIdList.size());

	int recentObjectListSize = m_recentObjectListSizeAttrPtr.IsValid()?
				istd::Max(*m_recentObjectListSizeAttrPtr, 1):
				1;
	I_ASSERT(recentObjectListSize >= 1);

	while (int(m_recentIdList.size()) > recentObjectListSize){
		I_ASSERT(m_storedInfoMap.find(m_recentIdList.front()) != m_storedInfoMap.end()); // object in recent list must exist in map

		m_storedInfoMap.erase(m_recentIdList.front());
		m_recentIdList.pop_front();

		I_ASSERT(m_storedInfoMap.size() == m_recentIdList.size());
	}

	I_ASSERT(!m_storedInfoMap.empty());
	I_ASSERT(!m_recentIdList.empty());
}


template <class SupplierInterface, class Product>
void TSupplierCompWrap<SupplierInterface, Product>::EnsureWorkFinished(I_DWORD objectId)
{
	TSupplierCompWrap::BeginNextObject(objectId);

	TSupplierCompWrap::GetWorkInfo(objectId, true);
}


template <class SupplierInterface, class Product>
int TSupplierCompWrap<SupplierInterface, Product>::GetWorkStatus(I_DWORD objectId) const
{
	const WorkInfo* infoPtr = GetWorkInfo(objectId, true);
	if (infoPtr != NULL){
		return infoPtr->status;
	}

	return WS_NONE;
}


template <class SupplierInterface, class Product>
double TSupplierCompWrap<SupplierInterface, Product>::GetWorkDurationTime(I_DWORD objectId) const
{
	const WorkInfo* infoPtr = GetWorkInfo(objectId, true);
	if (infoPtr != NULL){
		return infoPtr->durationTime;
	}

	return -1;
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

	BaseClass::OnComponentDestroyed();
}


// protected methods

template <class SupplierInterface, class Product>
bool TSupplierCompWrap<SupplierInterface, Product>::IsIdKnown(I_DWORD objectId) const
{
	return m_storedInfoMap.find(objectId) != m_storedInfoMap.end();
}


template <class SupplierInterface, class Product>
const typename TSupplierCompWrap<SupplierInterface, Product>::WorkInfo* TSupplierCompWrap<SupplierInterface, Product>::GetWorkInfo(
			I_DWORD objectId,
			bool ensureFinished) const
{
	StoredInfoMap::iterator foundIter = m_storedInfoMap.find(objectId);
	if (foundIter != m_storedInfoMap.end()){
		WorkInfo& workInfo = foundIter->second;

		if (ensureFinished && !workInfo.isDone){
			isys::ITimer* timerPtr = istd::GetService<isys::ITimer>();

			double beforeTime = 0;
			if (timerPtr != NULL){
				beforeTime = timerPtr->GetElapsed();
			}

			istd::CChangeNotifier notifier(const_cast<TSupplierCompWrap<SupplierInterface, Product>*>(this), CF_SUPPLIER_RESULTS);

			workInfo.status = ProduceObject(objectId, workInfo.product);

			workInfo.isDone = true;

			if (timerPtr != NULL){
				workInfo.durationTime = timerPtr->GetElapsed() - beforeTime;
			}
		}

		return &workInfo;
	}

	return NULL;
}


template <class SupplierInterface, class Product>
void TSupplierCompWrap<SupplierInterface, Product>::ResetProducedObjects()
{
	m_storedInfoMap.clear();
	m_recentIdList.clear();
}


// reimplemented (imod::CSingleModelObserverBase)

template <class SupplierInterface, class Product>
void TSupplierCompWrap<SupplierInterface, Product>::OnUpdate(int updateFlags, istd::IPolymorphic* updateParamsPtr)
{
	ResetProducedObjects();

	BaseClass2::OnUpdate(updateFlags, updateParamsPtr);
}


// pseudo-reimplemented (istd::IChangeable)

template <class SupplierInterface, class Product>
void TSupplierCompWrap<SupplierInterface, Product>::OnEndChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr)
{
	if ((changeFlags & CF_MODEL) != 0){
		ResetProducedObjects();
	}

	SupplierInterface::OnEndChanges(changeFlags, changeParamsPtr);
}


} // namespace iproc


#endif // !iproc_TSupplierCompWrap_included


