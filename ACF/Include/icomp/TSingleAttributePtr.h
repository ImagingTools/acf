#ifndef icomp_TSingleAttributePtr_included
#define icomp_TSingleAttributePtr_included


#include "icomp/IComponentContext.h"
#include "icomp/IRealAttributeStaticInfo.h"


namespace icomp
{


/**
	Pointer to component attribute.
	\internal
	Don't use direct this class, use macros I_ATTR and I_ASSIGN instead.
*/
template <typename Attribute>
class TSingleAttributePtr
{
public:
	typedef Attribute AttributeType;
	typedef void InterfaceType;

	TSingleAttributePtr();

	bool Init(const IComponent* ownerPtr, const IRealAttributeStaticInfo& staticInfo, const IComponentContext** realContextPtr = NULL);

	/**
		Check if this attribute is valid.
	*/
	bool IsValid() const;

	/**
		Access to internal attribute pointer.
	*/
	const Attribute* GetAttributePtr() const;

	/**
		Access to internal attribute pointer.
	*/
	const Attribute* operator->() const;

	/**
		Access to object pointed by internal pointer.
	*/
	typename const Attribute::ValueType& operator*() const;

protected:
	void SetAttribute(const Attribute* m_attributePtr);

private:
	const Attribute* m_attributePtr;
};


// public methods

template <typename Attribute>
TSingleAttributePtr<Attribute>::TSingleAttributePtr()
:	m_attributePtr(NULL)
{
}


template <typename Attribute>
bool TSingleAttributePtr<Attribute>::Init(const IComponent* ownerPtr, const IRealAttributeStaticInfo& staticInfo, const IComponentContext** realContextPtr)
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
bool TSingleAttributePtr<Attribute>::IsValid() const
{
	return (m_attributePtr != NULL);
}


template <typename Attribute>
const Attribute* TSingleAttributePtr<Attribute>::GetAttributePtr() const
{
	return m_attributePtr;
}


template <typename Attribute>
typename const Attribute* TSingleAttributePtr<Attribute>::operator->() const
{
	return m_attributePtr;
}


template <typename Attribute>
typename const Attribute::ValueType& TSingleAttributePtr<Attribute>::operator*() const
{
	I_ASSERT(m_attributePtr != NULL);	// operator* was called for invalid object, or no IsValid() check was called.

	return m_attributePtr->GetValue();
}


// protected methods

template <typename Attribute>
void TSingleAttributePtr<Attribute>::SetAttribute(const Attribute* m_attributePtr)
{
	m_attributePtr = attributePtr;
}


}//namespace icomp


#endif // !icomp_TSingleAttributePtr_included


