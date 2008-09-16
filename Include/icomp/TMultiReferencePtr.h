#ifndef icomp_TMultiReferencePtr_included
#define icomp_TMultiReferencePtr_included


#include "icomp/IComponentContext.h"
#include "icomp/TMultiAttributePtr.h"
#include "icomp/TInterfaceManipBase.h"
#include "icomp/CMultiReferenceAttribute.h"


namespace icomp
{


/**
	Pointer to list of referenced component objects.
	Don't use direct this class, use macros \c I_MULTI_REF and \c I_ASSIGN_MULTI_* instead.
*/
template <class Interface>
class TMultiReferencePtr: public TMultiAttributePtr<CMultiReferenceAttribute>, public TInterfaceManipBase<Interface>
{
public:
	typedef TMultiAttributePtr<CMultiReferenceAttribute> BaseClass;
	typedef Interface InterfaceType;

	TMultiReferencePtr();

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
	TMultiReferencePtr(const TMultiReferencePtr& ptr);

	bool EnsureInitialized() const;

private:
	const IComponent* m_definitionComponentPtr;

	typedef std::vector<Interface*> Components;
	mutable Components m_components;
	mutable bool m_isInitialized;
};


// public methods

template <class Interface>
TMultiReferencePtr<Interface>::TMultiReferencePtr()
:	m_definitionComponentPtr(NULL), m_isInitialized(false)
{
}


template <class Interface>
void TMultiReferencePtr<Interface>::Init(const IComponent* ownerPtr, const IRealAttributeStaticInfo& staticInfo)
{
	BaseClass::Init(ownerPtr, staticInfo, &m_definitionComponentPtr);

	m_components.clear();
}


template <class Interface>
bool TMultiReferencePtr<Interface>::IsValid() const
{
	return EnsureInitialized();
}


template <class Interface>
typename Interface* TMultiReferencePtr<Interface>::operator[](int index) const
{
	I_ASSERT(index >= 0);

	EnsureInitialized();

	I_ASSERT(index < int(m_components.size()));

	return m_components[index];
}


// protected methods

template <class Interface>
TMultiReferencePtr<Interface>::TMultiReferencePtr(const TMultiReferencePtr& ptr)
:	BaseClass(ptr),
	m_definitionComponentPtr(ptr.m_definitionComponentPtr),
	m_components(ptr.m_components),
	m_isInitialized(ptr.m_isInitialized)
{
}


template <class Interface>
bool TMultiReferencePtr<Interface>::EnsureInitialized() const
{
	bool retVal = false;

	if (!m_isInitialized && (m_definitionComponentPtr != NULL) && BaseClass::IsValid()){
		const IComponent* parentPtr = m_definitionComponentPtr->GetParentComponent();
		if (parentPtr != NULL){
			int attributesCount = BaseClass::GetCount();

			m_components.resize(attributesCount, NULL);

			retVal = true;

			for (int i = 0; i < attributesCount; ++i){
				const std::string& componentId = BaseClass::operator[](i);

				std::string baseId;
				std::string subId;
				SplitId(componentId, baseId, subId);

				IComponent* componentPtr = parentPtr->GetSubcomponent(componentId);

				m_components[i] = ExtractInterface(componentPtr, subId);
			}

			m_isInitialized = retVal;
		}
	}

	return m_isInitialized;
}


}//namespace icomp


#endif // !icomp_TMultiReferencePtr_included


