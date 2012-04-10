#ifndef icomp_TAttributeMember_included
#define icomp_TAttributeMember_included


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
class TAttributeMember
{
public:
	typedef Attribute AttributeType;
	typedef void InterfaceType;

	TAttributeMember();

	/**
		Initialize this attribute.
		\param	ownerPtr				pointer to parent component of this attribute.
		\param	staticInfo				static info structure creating this attribute.
		\param	definitionComponentPtr	optional pointer will be set with pointer to component defining this attribute.
										If this attribute was exported it will differ from parent component.
	*/
	bool Init(	const IComponent* ownerPtr,
				const IRealAttributeStaticInfo& staticInfo,
				const IComponent** definitionComponentPtr = NULL);

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
	const typename Attribute::ValueType& operator*() const;

protected:
	void SetAttribute(const Attribute* attributePtr);

private:
	const Attribute* m_attributePtr;
};


// public methods

template <typename Attribute>
TAttributeMember<Attribute>::TAttributeMember()
:	m_attributePtr(NULL)
{
}


template <typename Attribute>
bool TAttributeMember<Attribute>::Init(
			const IComponent* ownerPtr,
			const IRealAttributeStaticInfo& staticInfo,
			const IComponent** definitionComponentPtr)
{
	I_ASSERT(ownerPtr != NULL);

	const QByteArray& attributeId = staticInfo.GetAttributeId();
	const IComponentContext* componentContextPtr = ownerPtr->GetComponentContext();
	if (componentContextPtr != NULL){
		int definitionLevel = -1;
		const iser::IObject* attributePtr = componentContextPtr->GetAttribute(attributeId, &definitionLevel);
		m_attributePtr = dynamic_cast<const Attribute*>(attributePtr);

		if (m_attributePtr != NULL){
			I_ASSERT(definitionLevel >= 0);

			if (definitionComponentPtr != NULL){
				while (definitionLevel > 0){
					ownerPtr = ownerPtr->GetParentComponent();
					I_ASSERT(ownerPtr != NULL);

					--definitionLevel;
				}

				*definitionComponentPtr = ownerPtr;
			}

			return true;
		}
	}
	else{
		m_attributePtr = NULL;
	}

	return false;
}


template <typename Attribute>
bool TAttributeMember<Attribute>::IsValid() const
{
	return (m_attributePtr != NULL);
}


template <typename Attribute>
const Attribute* TAttributeMember<Attribute>::GetAttributePtr() const
{
	return m_attributePtr;
}


template <typename Attribute>
const Attribute* TAttributeMember<Attribute>::operator->() const
{
	return m_attributePtr;
}


template <typename Attribute>
const typename Attribute::ValueType& TAttributeMember<Attribute>::operator*() const
{
	I_ASSERT(m_attributePtr != NULL);	// operator* was called for invalid object, or no IsValid() check was called.

	return m_attributePtr->GetValue();
}


// protected methods

template <typename Attribute>
void TAttributeMember<Attribute>::SetAttribute(const Attribute* attributePtr)
{
	m_attributePtr = attributePtr;
}


} // namespace icomp


#endif // !icomp_TAttributeMember_included


