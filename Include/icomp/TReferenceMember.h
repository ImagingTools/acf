#pragma once

// Standard includes
#include <atomic>

// Qt includes
#include <QtCore/QMutex>

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

	\par Thread-Safety:
	This class uses double-checked locking with std::atomic for thread-safe lazy initialization.
	The pattern is safe because:
	- m_isInitialized is atomic with sequential consistency (default memory order)
	- m_interfacePtr is atomic
	- Once m_isInitialized is true, no further modifications occur to the cached data
	- The mutex protects the initialization phase
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

	mutable IComponentSharedPtr m_componentPtr;
	mutable std::atomic<Interface*> m_interfacePtr;
	mutable std::atomic<bool> m_isInitialized;

	mutable QRecursiveMutex m_mutex;
};


// public methods

template <class Interface>
TReferenceMember<Interface>::TReferenceMember()
	:m_definitionComponentPtr(NULL),
	m_isInitialized(false),
	m_interfacePtr(nullptr)
{
}


template <class Interface>
void TReferenceMember<Interface>::Init(const IComponent* ownerPtr, const IRealAttributeStaticInfo& staticInfo)
{
	QMutexLocker lock(&m_mutex);

	BaseClass::InitInternal(ownerPtr, staticInfo, &m_definitionComponentPtr);

	// CRITICAL: We reset the initialization flag BEFORE clearing pointers.
	// This ordering is REQUIRED for thread-safety:
	//
	// If another thread checks m_isInitialized without the lock and sees:
	// - true: The pointers are still valid (not yet cleared), safe to use
	// - false: Thread will acquire lock and re-initialize, finding cleared state
	//
	// If we cleared pointers first, another thread might see m_isInitialized==true
	// but access nullptr pointers, causing a crash.
	//
	// The lock ensures no thread is in EnsureInitialized() during this operation.
	m_isInitialized = false;
	m_interfacePtr = nullptr;
	m_componentPtr.reset();
}


template <class Interface>
bool TReferenceMember<Interface>::IsValid() const
{
	return EnsureInitialized();
}


template <class Interface>
bool TReferenceMember<Interface>::EnsureInitialized() const
{
	if (!m_isInitialized){
		QMutexLocker lock(&m_mutex);

		if (!m_isInitialized && (m_definitionComponentPtr != NULL) && BaseClass::IsValid()) {
			const ICompositeComponent* parentPtr = dynamic_cast<const ICompositeComponent*>(m_definitionComponentPtr->GetParentComponent());
			if (parentPtr != NULL) {
				const QByteArray& componentId = BaseClass::operator*();

				QByteArray baseId;
				QByteArray subId;
				BaseClass2::SplitId(componentId, baseId, subId);

				m_componentPtr = parentPtr->GetSubcomponent(baseId);
				if (m_componentPtr != nullptr) {
					m_interfacePtr = BaseClass2::ExtractInterface<Interface>(m_componentPtr.get(), subId);

					m_isInitialized = true;
				}
				else {
					qCritical("Component %s is defined, but definition component has no parent", BaseClass::operator*().constData());
				}
			}
		}
	}

	return (m_interfacePtr != NULL);
}


template <class Interface>
Interface* TReferenceMember<Interface>::GetPtr() const
{
	EnsureInitialized();

	return m_interfacePtr;
}


template <class Interface>
Interface* TReferenceMember<Interface>::operator->() const
{
	EnsureInitialized();

	Q_ASSERT(m_interfacePtr != NULL);

	return GetPtr();
}


// protected methods

template <class Interface>
TReferenceMember<Interface>::TReferenceMember(const TReferenceMember& ptr)
:	BaseClass(ptr),
	m_definitionComponentPtr(ptr.m_definitionComponentPtr),
	m_interfacePtr(nullptr),
	m_isInitialized(false)
{
	// Thread-safe copy: acquire lock on source object before copying
	QMutexLocker lock(&ptr.m_mutex);
	m_componentPtr = ptr.m_componentPtr;
	m_interfacePtr = ptr.m_interfacePtr.load();
	m_isInitialized = ptr.m_isInitialized.load();
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


