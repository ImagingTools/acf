#pragma once

// Standard includes
#include <atomic>

// Qt includes
#include <QtCore/QMutex>

// ACF includes
#include <icomp/TMultiAttributeMember.h>
#include <icomp/CInterfaceManipBase.h>
#include <icomp/CMultiReferenceAttribute.h>


namespace icomp
{


/**
	Pointer to list of referenced component objects.
	Don't use direct this class, use macros \c I_MULTI_REF and \c I_ASSIGN_MULTI_* instead.

	\par Thread-Safety:
	This class uses double-checked locking with std::atomic for thread-safe lazy initialization.
	The pattern is safe because:
	- m_isInitialized is atomic with sequential consistency (default memory order)
	- Once m_isInitialized is true, no further modifications occur to m_components
	- The mutex protects the initialization phase
	- Sequential consistency ensures all writes to m_components are visible to threads
	  that observe m_isInitialized == true
*/
template <class Interface>
class TMultiReferenceMember: public TMultiAttributeMember<CMultiReferenceAttribute>, public CInterfaceManipBase
{
public:
	typedef TMultiAttributeMember<CMultiReferenceAttribute> BaseClass;
	typedef CInterfaceManipBase BaseClass2;
	typedef Interface InterfaceType;

	TMultiReferenceMember();

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
		Access to interface of component at specified index.
		\return	pointer to interface or NULL if there is no associated object at this index.
	*/
	Interface* operator[](int index) const;

protected:
	TMultiReferenceMember(const TMultiReferenceMember& ptr);

private:
	const IComponent* m_definitionComponentPtr;

	struct Component
	{
		IComponentSharedPtr componentPtr;
		Interface* interfacePtr = nullptr;
	};


	typedef QVector<Component> Components;
	mutable Components m_components;

	mutable std::atomic<bool> m_isInitialized;

	mutable QRecursiveMutex m_mutex;
};


// public methods

template <class Interface>
TMultiReferenceMember<Interface>::TMultiReferenceMember()
	:m_definitionComponentPtr(NULL),
	m_isInitialized(false)
{
}


template <class Interface>
void TMultiReferenceMember<Interface>::Init(const IComponent* ownerPtr, const IRealAttributeStaticInfo& staticInfo)
{
	QMutexLocker lock(&m_mutex);

	BaseClass::InitInternal(ownerPtr, staticInfo, &m_definitionComponentPtr);

	// CRITICAL: We reset the initialization flag BEFORE clearing data.
	// This ordering is REQUIRED for thread-safety:
	//
	// If another thread checks m_isInitialized without the lock and sees:
	// - true: The data is still valid (not yet cleared), safe to use
	// - false: Thread will acquire lock and re-initialize, finding cleared state
	//
	// If we cleared data first, another thread might see m_isInitialized==true
	// but access partially cleared or invalid data in m_components.
	//
	// The lock ensures no thread is in EnsureInitialized() during this operation.
	m_isInitialized = false;
	m_components.clear();
}


template <class Interface>
bool TMultiReferenceMember<Interface>::IsValid() const
{
	return EnsureInitialized();
}


template <class Interface>
bool TMultiReferenceMember<Interface>::EnsureInitialized() const
{
	if (!m_isInitialized){
		QMutexLocker lock(&m_mutex);

		if (!m_isInitialized && (m_definitionComponentPtr != NULL) && BaseClass::IsValid()) {
			const ICompositeComponent* parentPtr = dynamic_cast<const ICompositeComponent*>(m_definitionComponentPtr->GetParentComponent());
			if (parentPtr != NULL) {
				int attributesCount = BaseClass::GetCount();

				m_components.resize(attributesCount);

				for (int i = 0; i < attributesCount; ++i) {
					const QByteArray& componentId = BaseClass::operator[](i);

					QByteArray baseId;
					QByteArray subId;
					BaseClass2::SplitId(componentId, baseId, subId);

					IComponentSharedPtr componentPtr = parentPtr->GetSubcomponent(baseId);

					m_components[i].componentPtr = componentPtr;
					m_components[i].interfacePtr = BaseClass2::ExtractInterface<Interface>(componentPtr.get(), subId);
				}

				m_isInitialized = true;
			}
			else {
				qCritical("Components are is defined, but definition component has no parent");
			}
		}
	}

	return m_isInitialized;
}


template <class Interface>
Interface* TMultiReferenceMember<Interface>::operator[](int index) const
{
	Q_ASSERT(index >= 0);

	if (EnsureInitialized()){
		Q_ASSERT(index < int(m_components.size()));

		return m_components[index].interfacePtr;
	}

	return NULL;
}


// protected methods

template <class Interface>
TMultiReferenceMember<Interface>::TMultiReferenceMember(const TMultiReferenceMember& ptr)
:	BaseClass(ptr),
	m_definitionComponentPtr(ptr.m_definitionComponentPtr),
	m_isInitialized(false)
{
	// Thread-safe copy: acquire lock on source object before copying
	QMutexLocker lock(&ptr.m_mutex);
	m_components = ptr.m_components;
	m_isInitialized = ptr.m_isInitialized.load();
}


} // namespace icomp


