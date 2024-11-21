#ifndef icomp_TReferenceMember_included
#define icomp_TReferenceMember_included


// ACF includes
#include <icomp/IComponent.h>
#include <icomp/TAttributeMember.h>
#include <icomp/CInterfaceManipBase.h>
#include <icomp/CReferenceAttribute.h>


namespace icomp
{


/**
	Pointer to referenced component object.
	Don't use direct this class, use macros \c I_REF and \c I_ASSIGN instead.
*/
template <class Interface>
class TReferenceMember:
			public TAttributeMember<CReferenceAttribute>,
			protected CInterfaceManipBase
{
public:
	typedef TAttributeMember<CReferenceAttribute> BaseClass;
	typedef CInterfaceManipBase BaseClass2;
	typedef Interface InterfaceType;

	TReferenceMember();

	void Init(const IComponent* ownerPtr, const IRealAttributeStaticInfo& staticInfo);

	/**
		Check if this reference can be resolved.
		It calls \c EnsureInitialized().
	*/
	bool IsValid() const;

	/**
		Ensure that initlization process is closed.
		Do the same as \c IsValid(), for convinience only (to provide better code clarify).
	*/
	bool EnsureInitialized() const;

	/**
		Direct cccess to internal pointer.
	*/
	Interface* GetPtr() const;

	/**
		Access to internal pointer.
	*/
	Interface* operator->() const;

protected:
	TReferenceMember(const TReferenceMember& ptr);

private:
	const IComponent* m_definitionComponentPtr;

	mutable std::atomic<Interface*> m_componentPtr;
	mutable std::atomic<bool> m_isInitialized;
};


// public methods

template <class Interface>
TReferenceMember<Interface>::TReferenceMember()
:	m_definitionComponentPtr(NULL), m_componentPtr(NULL), m_isInitialized(false)
{
}


template <class Interface>
void TReferenceMember<Interface>::Init(const IComponent* ownerPtr, const IRealAttributeStaticInfo& staticInfo)
{
	BaseClass::InitInternal(ownerPtr, staticInfo, &m_definitionComponentPtr);

	m_componentPtr = NULL;
}


template <class Interface>
bool TReferenceMember<Interface>::IsValid() const
{
	return EnsureInitialized();
}


template <class Interface>
bool TReferenceMember<Interface>::EnsureInitialized() const
{
	if (!m_isInitialized && (m_definitionComponentPtr != NULL) && BaseClass::IsValid()){
		const ICompositeComponent* parentPtr = m_definitionComponentPtr->GetParentComponent();
		if (parentPtr != NULL){
			const QByteArray& componentId = BaseClass::operator*();

			QByteArray baseId;
			QByteArray subId;
			BaseClass2::SplitId(componentId, baseId, subId);

			IComponent* componentPtr = parentPtr->GetSubcomponent(baseId);
			if (componentPtr != NULL){
				m_componentPtr = BaseClass2::ExtractInterface<Interface>(componentPtr, subId);
			}

			m_isInitialized = true;
		}
		else{
			qCritical("Component %s is defined, but definition component has no parent", BaseClass::operator*().constData());
		}
	}

	return (m_componentPtr != NULL);
}


template <class Interface>
Interface* TReferenceMember<Interface>::GetPtr() const
{
	EnsureInitialized();

	return m_componentPtr;
}


template <class Interface>
Interface* TReferenceMember<Interface>::operator->() const
{
	EnsureInitialized();
	Q_ASSERT(m_componentPtr != NULL);

	return GetPtr();
}


// protected methods

template <class Interface>
TReferenceMember<Interface>::TReferenceMember(const TReferenceMember& ptr)
:	BaseClass(ptr),
	m_definitionComponentPtr(ptr.m_definitionComponentPtr),
	m_componentPtr(ptr.m_componentPtr),
	m_isInitialized(ptr.m_isInitialized)
{
}


} // namespace icomp


inline void operator*(const icomp::TReferenceMember<void>& /*ref*/)
{
}


template <class Interface>
inline Interface& operator*(const icomp::TReferenceMember<Interface>& ref)
{
	return *ref.GetPtr();
}


#endif // !icomp_TReferenceMember_included


