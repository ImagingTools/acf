#ifndef icomp_TMultiAttributeMember_included
#define icomp_TMultiAttributeMember_included


// ACF includes
#include "iattr/TMultiAttribute.h"
#include "icomp/IComponentContext.h"


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

	/**
		Internal initialize of attribute.
		\param	ownerPtr				pointer to parent component of this attribute.
		\param	staticInfo				static info structure creating this attribute.
		\param	definitionComponentPtr	optional pointer will be set with pointer to component defining this attribute.
										If this attribute was exported it will differ from parent component.
	*/
	void Init(const IComponent* ownerPtr, const IRealAttributeStaticInfo& staticInfo);

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

	/**
		Find attribute value.
		\return Index of the found element, or -1 if no value was found.
	*/
	int FindValue(const AttributeValueType& value) const;

protected:
	bool InitInternal(
				const IComponent* ownerPtr,
				const IRealAttributeStaticInfo& staticInfo,
				const IComponent** definitionComponentPtr);


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
void TMultiAttributeMemberBase<Attribute>::Init(
			const IComponent* ownerPtr,
			const IRealAttributeStaticInfo& staticInfo)
{
	InitInternal(ownerPtr, staticInfo, NULL);
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
	Q_ASSERT(index >= 0);
	Q_ASSERT(index < GetCount());
	Q_ASSERT(m_attributePtr != NULL);	// operator* was called for invalid object, or no IsValid() check was called.
	Q_ASSERT(m_attributePtr->GetValuesCount() == GetCount());

	return m_attributePtr->GetValueAt(index);
}


template <typename Attribute>
int TMultiAttributeMemberBase<Attribute>::FindValue(const AttributeValueType& value) const
{
	if (m_attributePtr != NULL){
		return m_attributePtr->FindValue(value);
	}

	return -1;
}


// protected methods

template <typename Attribute>
bool TMultiAttributeMemberBase<Attribute>::InitInternal(
			const IComponent* ownerPtr,
			const IRealAttributeStaticInfo& staticInfo,
			const IComponent** definitionComponentPtr)
{
	Q_ASSERT(ownerPtr != NULL);

	const QByteArray& attributeId = staticInfo.GetAttributeId();
	const IComponentContext* componentContextPtr = ownerPtr->GetComponentContext();
	if (componentContextPtr != NULL){
		int definitionLevel = -1;
		const iser::IObject* attributePtr = componentContextPtr->GetAttribute(attributeId, &definitionLevel);
		m_attributePtr = dynamic_cast<const Attribute*>(attributePtr);

		if (m_attributePtr != NULL){
			Q_ASSERT(definitionLevel >= 0);

			if (definitionComponentPtr != NULL){
				while (definitionLevel > 0){
					ownerPtr = ownerPtr->GetParentComponent();
					Q_ASSERT(ownerPtr != NULL);

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


// other constructs used for special template for QString attribute

template <typename Attribute>
class TMultiAttributeMember: public TMultiAttributeMemberBase<Attribute>
{
};


template <>
class TMultiAttributeMember< iattr::TMultiAttribute<QString> >: public TMultiAttributeMemberBase< iattr::TMultiAttribute<QString> >
{
public:
	QString operator[](int index) const
	{
		return QCoreApplication::translate("Attribute", TMultiAttributeMemberBase< iattr::TMultiAttribute<QString> >::operator[](index).toLatin1());
	}
};


} // namespace icomp


#endif // !icomp_TMultiAttributeMember_included


