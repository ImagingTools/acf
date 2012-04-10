#ifndef icomp_TMultiReferenceMember_included
#define icomp_TMultiReferenceMember_included


// ACF includes
#include "icomp/TMultiAttributeMember.h"
#include "icomp/CInterfaceManipBase.h"
#include "icomp/CMultiReferenceAttribute.h"


namespace icomp
{


/**
	Pointer to list of referenced component objects.
	Don't use direct this class, use macros \c I_MULTI_REF and \c I_ASSIGN_MULTI_* instead.
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
	*/
	bool IsValid() const;

	/**
		Access to interface of component at specified index.
		\return	pointer to interface or NULL if there is no associated object at this index.
	*/
	Interface* operator[](int index) const;

protected:
	TMultiReferenceMember(const TMultiReferenceMember& ptr);

	bool EnsureInitialized() const;

private:
	const IComponent* m_definitionComponentPtr;

	typedef QVector<Interface*> Components;
	mutable Components m_components;
	mutable bool m_isInitialized;
};


// public methods

template <class Interface>
TMultiReferenceMember<Interface>::TMultiReferenceMember()
:	m_definitionComponentPtr(NULL), m_isInitialized(false)
{
}


template <class Interface>
void TMultiReferenceMember<Interface>::Init(const IComponent* ownerPtr, const IRealAttributeStaticInfo& staticInfo)
{
	BaseClass::Init(ownerPtr, staticInfo, &m_definitionComponentPtr);

	m_components.clear();
}


template <class Interface>
bool TMultiReferenceMember<Interface>::IsValid() const
{
	return EnsureInitialized();
}


template <class Interface>
Interface* TMultiReferenceMember<Interface>::operator[](int index) const
{
	I_ASSERT(index >= 0);

	EnsureInitialized();

	I_ASSERT(index < int(m_components.size()));

	return m_components[index];
}


// protected methods

template <class Interface>
TMultiReferenceMember<Interface>::TMultiReferenceMember(const TMultiReferenceMember& ptr)
:	BaseClass(ptr),
	m_definitionComponentPtr(ptr.m_definitionComponentPtr),
	m_components(ptr.m_components),
	m_isInitialized(ptr.m_isInitialized)
{
}


template <class Interface>
bool TMultiReferenceMember<Interface>::EnsureInitialized() const
{
	bool retVal = false;

	if (!m_isInitialized && (m_definitionComponentPtr != NULL) && BaseClass::IsValid()){
		const ICompositeComponent* parentPtr = m_definitionComponentPtr->GetParentComponent();
		if (parentPtr != NULL){
			int attributesCount = BaseClass::GetCount();

			m_components.resize(attributesCount);

			retVal = true;

			for (int i = 0; i < attributesCount; ++i){
				const QByteArray& componentId = BaseClass::operator[](i);

				QByteArray baseId;
				QByteArray subId;
				BaseClass2::SplitId(componentId, baseId, subId);

				IComponent* componentPtr = parentPtr->GetSubcomponent(baseId);

				m_components[i] = BaseClass2::ExtractInterface<Interface>(componentPtr, subId);
			}

			m_isInitialized = retVal;
		}
	}

	return m_isInitialized;
}


} // namespace icomp


#endif // !icomp_TMultiReferenceMember_included


