#ifndef icomp_TAttributePtr_included
#define icomp_TAttributePtr_included


#include "icomp/IComponentContext.h"


namespace icomp
{


/**
	Pointer to component attribute.
	Don't use direct this class, use macros I_ATTR and I_ASSIGN instead.
*/
template <class Attribute>
class TAttributePtr
{
public:
	typedef Attribute AttributeType;

	TAttributePtr();

	bool Init(const IComponent* ownerPtr, const IAttributeStaticInfo& staticInfo, const IComponentContext** realContextPtr = NULL);

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
	const Attribute& operator*() const;

protected:
	void SetAttribute(const Attribute* m_attributePtr);

private:
	const Attribute* m_attributePtr;
};


// public methods

template <class Attribute>
TAttributePtr<Attribute>::TAttributePtr()
:	m_attributePtr(NULL)
{
}


template <class Attribute>
bool TAttributePtr<Attribute>::Init(const IComponent* ownerPtr, const IAttributeStaticInfo& staticInfo, const IComponentContext** realContextPtr)
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


template <class Attribute>
bool TAttributePtr<Attribute>::IsValid() const
{
	return (m_attributePtr != NULL);
}


template <class Attribute>
const Attribute* TAttributePtr<Attribute>::GetAttributePtr() const
{
	return m_attributePtr;
}


template <class Attribute>
const Attribute* TAttributePtr<Attribute>::operator->() const
{
	return m_attributePtr;
}


template <class Attribute>
const Attribute& TAttributePtr<Attribute>::operator*() const
{
	I_ASSERT(m_attributePtr != NULL);	// operator* was called for invalid object, or no IsValid() check was called.

	return *m_attributePtr;
}


// protected methods

template <class Attribute>
void TAttributePtr<Attribute>::SetAttribute(const Attribute* m_attributePtr)
{
	m_attributePtr = attributePtr;
}


}//namespace icomp


#endif // !icomp_TAttributePtr_included


