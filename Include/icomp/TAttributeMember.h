#ifndef icomp_TAttributeMember_included
#define icomp_TAttributeMember_included


// Qt includes
#include <QtCore/QCoreApplication>

// ACF includes
#include <iattr/TAttribute.h>
#include <icomp/ICompositeComponent.h>
#include <icomp/IComponentContext.h>
#include <icomp/IRealAttributeStaticInfo.h>
#include <icomp/CComponentContext.h>


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
	*/
	void Init(const IComponent* ownerPtr, const IRealAttributeStaticInfo& staticInfo);

	/**
		Check if this attribute is valid.
	*/
	bool IsValid() const;

	/**
		Access to internal attribute pointer.
	*/
	const Attribute* GetAttributePtr() const;
	/**
		Get value of attribute.
		The \c String attributes will not be translated.
	*/
	const AttributeValueType& GetOriginalValue() const;

	/**
		Access to internal attribute pointer.
	*/
	const Attribute* operator->() const;

	/**
		Get value of attribute.
		Access to object pointed by internal pointer or translated values for \c String.
	*/
	const AttributeValueType& operator*() const;

protected:
	void SetAttribute(const Attribute* attributePtr);

	/**
		Internal initialize of attribute.
		\param	ownerPtr				pointer to parent component of this attribute.
		\param	staticInfo				static info structure creating this attribute.
		\param	definitionComponentPtr	optional pointer will be set with pointer to component defining this attribute.
										If this attribute was exported it will differ from parent component.
	*/
	bool InitInternal(	const IComponent* ownerPtr,
				const IRealAttributeStaticInfo& staticInfo,
				const IComponent** definitionComponentPtr);

private:
	const Attribute* m_attributePtr;
	bool m_isAssigned;
};


// public methods

template <typename Attribute>
TAttributeMemberBase<Attribute>::TAttributeMemberBase()
:	m_attributePtr(NULL),
	m_isAssigned(false)
{
}


template <typename Attribute>
void TAttributeMemberBase<Attribute>::Init(
			const IComponent* ownerPtr,
			const IRealAttributeStaticInfo& staticInfo)
{
	InitInternal(ownerPtr, staticInfo, NULL);
}

template <typename Attribute>
bool TAttributeMemberBase<Attribute>::IsValid() const
{
	Q_ASSERT_X(m_isAssigned, "Component initialization", "No I_ASSIGN used or attribute is used out of component context");

	return (m_attributePtr != NULL);
}


template <typename Attribute>
const Attribute* TAttributeMemberBase<Attribute>::GetAttributePtr() const
{
	Q_ASSERT_X(m_isAssigned, "Component initialization", "No I_ASSIGN used or attribute is used out of component context");

	return m_attributePtr;
}


template <typename Attribute>
const typename TAttributeMemberBase<Attribute>::AttributeValueType& TAttributeMemberBase<Attribute>::GetOriginalValue() const
{
	Q_ASSERT(m_attributePtr != NULL);	// GetOriginalValue() was called for invalid object, or no IsValid() check was called.

	return m_attributePtr->GetValue();
}


template <typename Attribute>
const Attribute* TAttributeMemberBase<Attribute>::operator->() const
{
	Q_ASSERT_X(m_isAssigned, "Component initialization", "No I_ASSIGN used or attribute is used out of component context");

	return m_attributePtr;
}


template <typename Attribute>
const typename TAttributeMemberBase<Attribute>::AttributeValueType& TAttributeMemberBase<Attribute>::operator*() const
{
	Q_ASSERT(m_attributePtr != NULL);	// operator* was called for invalid object, or no IsValid() check was called.

	return m_attributePtr->GetValue();
}


// protected methods

template <typename Attribute>
void TAttributeMemberBase<Attribute>::SetAttribute(const Attribute* attributePtr)
{
	m_attributePtr = attributePtr;
}


template <typename Attribute>
bool TAttributeMemberBase<Attribute>::InitInternal(
			const IComponent* ownerPtr,
			const IRealAttributeStaticInfo& staticInfo,
			const IComponent** definitionComponentPtr)
{
	Q_ASSERT(ownerPtr != NULL);

	m_isAssigned = true;

	const QByteArray& attributeId = staticInfo.GetAttributeId();
	const IComponentContext* componentContextPtr = ownerPtr->GetComponentContext();
	if (componentContextPtr != NULL){
		if (definitionComponentPtr != NULL){
			int definitionLevel = -1;
			const iser::IObject* attributePtr = componentContextPtr->GetAttribute(attributeId, &definitionLevel);
			if (attributePtr != NULL){
				m_attributePtr = dynamic_cast<const Attribute*>(attributePtr);

				if (m_attributePtr != NULL){
					Q_ASSERT(definitionLevel >= 0);

					while (definitionLevel > 0){
						ownerPtr = ownerPtr->GetParentComponent();
						Q_ASSERT(ownerPtr != NULL);

						--definitionLevel;
					}

					*definitionComponentPtr = ownerPtr;

					return true;
				}
				else{
					qCritical(	"Component '%s': Attribute '%s' type inconsistence!",
								CComponentContext::GetHierarchyAddress(componentContextPtr).constData(),
								attributeId.constData());
				}
			}
		}
		else{
			const iser::IObject* attributePtr = componentContextPtr->GetAttribute(attributeId, NULL);
			m_attributePtr = dynamic_cast<const Attribute*>(attributePtr);

			if (m_attributePtr == NULL){
				if (attributePtr != NULL){
					qCritical(	"Component %s: Attribute %s exists in the component context but has a wrong type",
								CComponentContext::GetHierarchyAddress(componentContextPtr).constData(),
								attributeId.constData());
				}
			}

			return (m_attributePtr != NULL);
		}
	}
	else{
		qCritical(	"Error during resolving of attribute: %s in component %s: Component context not set",
					CComponentContext::GetHierarchyAddress(componentContextPtr).constData(),
					attributeId.constData());

		m_attributePtr = NULL;
	}

	return false;
}


// other constructs used for special template for QString attribute

template <typename Attribute>
class TAttributeMember: public TAttributeMemberBase<Attribute>
{
};


template <>
class TAttributeMember< iattr::TAttribute<QString> >: public TAttributeMemberBase< iattr::TAttribute<QString> >
{
public:
	QString operator*() const
	{
		return QCoreApplication::translate("Attribute", TAttributeMemberBase< iattr::TAttribute<QString> >::operator*().toUtf8());
	}
};


} // namespace icomp


#endif // !icomp_TAttributeMember_included


