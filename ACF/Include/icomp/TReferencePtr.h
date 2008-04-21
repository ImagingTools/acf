#ifndef icomp_TReferencePtr_included
#define icomp_TReferencePtr_included


#include "icomp/IComponentContext.h"
#include "icomp/TSingleAttributePtr.h"
#include "icomp/TInterfaceManipBase.h"
#include "icomp/CReferenceAttribute.h"


namespace icomp
{


/**
	Pointer to referenced component object.
	Don't use direct this class, use macros \c I_REF and \c I_ASSIGN instead.
*/
template <class Interface>
class TReferencePtr: public TSingleAttributePtr<CReferenceAttribute>, public TInterfaceManipBase<Interface>
{
public:
	typedef TSingleAttributePtr<CReferenceAttribute> BaseClass;
	typedef Interface InterfaceType;

	TReferencePtr();

	void Init(const IComponent* ownerPtr, const IAttributeStaticInfo& staticInfo);

	/**
		Check if this reference can be resolved.
	*/
	bool IsValid() const;

	/**
		Direct cccess to internal pointer.
	*/
	Interface* GetPtr() const;

	/**
		Access to internal pointer.
	*/
	Interface* operator->() const;

	/**
		Access to object pointed by internal pointer.
	*/
	Interface& operator*() const;

protected:
	TReferencePtr(const TReferencePtr& ptr);

	bool EnsureInitialized() const;

private:
	const IComponentContext* m_realContextPtr;

	mutable Interface* m_componentPtr;
	mutable bool m_isInitialized;
};


// public methods

template <class Interface>
TReferencePtr<Interface>::TReferencePtr()
:	m_realContextPtr(NULL), m_componentPtr(NULL), m_isInitialized(false)
{
}


template <class Interface>
void TReferencePtr<Interface>::Init(const IComponent* ownerPtr, const IAttributeStaticInfo& staticInfo)
{
	BaseClass::Init(ownerPtr, staticInfo, &m_realContextPtr);

	m_componentPtr = NULL;
}


template <class Interface>
bool TReferencePtr<Interface>::IsValid() const
{
	return EnsureInitialized();
}



template <class Interface>
typename Interface* TReferencePtr<Interface>::GetPtr() const
{
	EnsureInitialized();

	return m_componentPtr;
}


template <class Interface>
typename Interface* TReferencePtr<Interface>::operator->() const
{
	EnsureInitialized();
	I_ASSERT(m_componentPtr != NULL);

	return GetPtr();
}


template <class Interface>
typename Interface& TReferencePtr<Interface>::operator*() const
{
	EnsureInitialized();
	I_ASSERT(m_componentPtr != NULL);

	return *m_componentPtr;
}


// protected methods

template <class Interface>
TReferencePtr<Interface>::TReferencePtr(const TReferencePtr& ptr)
:	BaseClass(ptr),
	m_realContextPtr(ptr.m_realContextPtr),
	m_componentPtr(ptr.m_componentPtr),
	m_isInitialized(ptr.m_isInitialized)
{
}


template <class Interface>
bool TReferencePtr<Interface>::EnsureInitialized() const
{
	if (!m_isInitialized && (m_realContextPtr != NULL) && BaseClass::IsValid()){
		const IComponentContext* parentPtr = m_realContextPtr->GetParentContext();
		if (parentPtr != NULL){
			const std::string& componentId = BaseClass::operator*();

			std::string baseId;
			std::string subId;
			SplitId(componentId, baseId, subId);

			IComponent* componentPtr = parentPtr->GetSubcomponent(baseId);

			m_componentPtr = ExtractInterface(componentPtr, subId);

			m_isInitialized = true;
		}
	}

	return (m_componentPtr != NULL);
}


}//namespace icomp


#endif // !icomp_TReferencePtr_included


