#ifndef icomp_TMultiAttributeMember_included
#define icomp_TMultiAttributeMember_included


// ACF includes
#include "icomp/IComponentContext.h"
#include "icomp/TMultiAttribute.h"


namespace icomp
{


/**
	Pointer to component attribute.
	\internal
	Don't use direct this class, use macros \c I_ATTR and \c I_ASSIGN_MULTI_* instead.
*/
template <typename Attribute>
class TMultiAttributeMemberBase
{
public:
    typedef Attribute AttributeType;
    typedef typename Attribute::ValueType AttributeValueType;
    typedef void InterfaceType;

	TMultiAttributeMemberBase();

	bool Init(	const IComponent* ownerPtr,
				const IRealAttributeStaticInfo& staticInfo,
				const IComponent** definitionComponentPtr = NULL);

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
    const AttributeValueType& operator[](int index) const;

private:
	const Attribute* m_attributePtr;
};


// public methods

template <typename Attribute>
TMultiAttributeMemberBase<Attribute>::TMultiAttributeMemberBase()
:	m_attributePtr(NULL)
{
}


template <typename Attribute>
bool TMultiAttributeMemberBase<Attribute>::Init(
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
bool TMultiAttributeMemberBase<Attribute>::IsValid() const
{
	return (m_attributePtr != NULL);
}


template <typename Attribute>
int TMultiAttributeMemberBase<Attribute>::GetCount() const
{
	if (m_attributePtr != NULL){
		return m_attributePtr->GetValuesCount();
	}
	else{
		return 0;
	}
}


template <typename Attribute>
const typename TMultiAttributeMemberBase<Attribute>::AttributeValueType& TMultiAttributeMemberBase<Attribute>::operator[](int index) const
{
	I_ASSERT(index >= 0);
	I_ASSERT(index < GetCount());
	I_ASSERT(m_attributePtr != NULL);	// operator* was called for invalid object, or no IsValid() check was called.
	I_ASSERT(m_attributePtr->GetValuesCount() == GetCount());

	return m_attributePtr->GetValueAt(index);
}


// other constructs used for special template for QString attribute

template <typename Attribute>
class TMultiAttributeMember: public TMultiAttributeMemberBase<Attribute>
{
};


template <>
class TMultiAttributeMember< TMultiAttribute<QString> >: public TMultiAttributeMemberBase< TMultiAttribute<QString> >
{
public:
	QString operator[](int index) const
	{
		return QCoreApplication::translate("Attribute", TMultiAttributeMemberBase< TMultiAttribute<QString> >::operator[](index).toAscii());
	}
};


} // namespace icomp


#endif // !icomp_TMultiAttributeMember_included


