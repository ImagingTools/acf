#ifndef icomp_TMultiAttributePtr_included
#define icomp_TMultiAttributePtr_included


#include "icomp/IComponentContext.h"


namespace icomp
{


/**
	Pointer to component attribute.
	\internal
	Don't use direct this class, use macros \c I_ATTR and \c I_ASSIGN_MULTI_* instead.
*/
template <typename Attribute>
class TMultiAttributePtr
{
public:
	typedef Attribute AttributeType;
	typedef void InterfaceType;

	TMultiAttributePtr();

	bool Init(const IComponent* ownerPtr, const IRealAttributeStaticInfo& staticInfo, const IComponentContext** realContextPtr = NULL);

	/**
		Check if this attribute is valid.
	*/
	bool IsValid() const;

	/**
		Get number of attributes.
	*/
	int GetCount() const;

	/**
		Access to object pointed by internal pointer.
	*/
	typename const Attribute::ValueType& operator[](int index) const;

private:
	const Attribute* m_attributePtr;
};


// public methods

template <typename Attribute>
TMultiAttributePtr<Attribute>::TMultiAttributePtr()
:	m_attributePtr(NULL)
{
}


template <typename Attribute>
bool TMultiAttributePtr<Attribute>::Init(const IComponent* ownerPtr, const IRealAttributeStaticInfo& staticInfo, const IComponentContext** realContextPtr)
{
	I_ASSERT(ownerPtr != NULL);

	const std::string& attributeId = staticInfo.GetAttributeId();
	const IComponentContext* componentContextPtr = ownerPtr->GetComponentContext();
	if (componentContextPtr != NULL){
		const iser::ISerializable* attributePtr = componentContextPtr->GetAttribute(attributeId, realContextPtr);
		m_attributePtr = dynamic_cast<const Attribute*>(attributePtr);
	}

	return (m_attributePtr != NULL);
}


template <typename Attribute>
bool TMultiAttributePtr<Attribute>::IsValid() const
{
	return (m_attributePtr != NULL);
}


template <typename Attribute>
int TMultiAttributePtr<Attribute>::GetCount() const
{
	if (m_attributePtr != NULL){
		return m_attributePtr->GetValuesCount();
	}
	else{
		return 0;
	}
}


template <typename Attribute>
typename const Attribute::ValueType& TMultiAttributePtr<Attribute>::operator[](int index) const
{
	I_ASSERT(index >= 0);
	I_ASSERT(index < GetCount());
	I_ASSERT(m_attributePtr != NULL);	// operator* was called for invalid object, or no IsValid() check was called.
	I_ASSERT(m_attributePtr->GetValuesCount() == GetCount());

	return m_attributePtr->GetValueAt(index);
}


}//namespace icomp


#endif // !icomp_TMultiAttributePtr_included


