#ifndef icomp_TAttributeMember_included
#define icomp_TAttributeMember_included


// Qt includes
#include <QtCore/QCoreApplication>

// ACF includes
#include "icomp/IComponentContext.h"
#include "icomp/IRealAttributeStaticInfo.h"
#include "icomp/TAttribute.h"


namespace icomp
{


/**
	Pointer to component attribute.
	\internal
	Don't use direct this class, use macros I_ATTR and I_ASSIGN instead.
*/
template <typename Attribute>
class TAttributeMemberBase
{
public:
    typedef Attribute AttributeType;
    typedef typename Attribute::ValueType AttributeValueType;
    typedef void InterfaceType;

	TAttributeMemberBase();

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
    const AttributeValueType& operator*() const;

protected:
	void SetAttribute(const Attribute* attributePtr);

private:
	const Attribute* m_attributePtr;
};


// public methods

template <typename Attribute>
TAttributeMemberBase<Attribute>::TAttributeMemberBase()
:	m_attributePtr(NULL)
{
}


template <typename Attribute>
bool TAttributeMemberBase<Attribute>::Init(
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
bool TAttributeMemberBase<Attribute>::IsValid() const
{
	return (m_attributePtr != NULL);
}


template <typename Attribute>
const Attribute* TAttributeMemberBase<Attribute>::GetAttributePtr() const
{
	return m_attributePtr;
}


template <typename Attribute>
const Attribute* TAttributeMemberBase<Attribute>::operator->() const
{
	return m_attributePtr;
}


template <typename Attribute>
const typename TAttributeMemberBase<Attribute>::AttributeValueType& TAttributeMemberBase<Attribute>::operator*() const
{
	I_ASSERT(m_attributePtr != NULL);	// operator* was called for invalid object, or no IsValid() check was called.

	return m_attributePtr->GetValue();
}


// protected methods

template <typename Attribute>
void TAttributeMemberBase<Attribute>::SetAttribute(const Attribute* attributePtr)
{
	m_attributePtr = attributePtr;
}


// other constructs used for special template for QString attribute

template <typename Attribute>
class TAttributeMember: public TAttributeMemberBase<Attribute>
{
};


template <>
class TAttributeMember< TAttribute<QString> >: public TAttributeMemberBase< TAttribute<QString> >
{
public:
	QString operator*() const
	{
		return QCoreApplication::translate("Attribute", TAttributeMemberBase< TAttribute<QString> >::operator*().toAscii());
	}
};


} // namespace icomp


#endif // !icomp_TAttributeMember_included


