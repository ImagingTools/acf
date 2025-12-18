#pragma once


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
	Don't use direct this class, use macros \c I_ATTR, \c I_TEXTATTR and \c I_ASSIGN instead.
*/
template <typename Attribute>
class TAttributeMember
{
public:
	typedef Attribute AttributeType;
	typedef typename Attribute::ValueType AttributeValueType;
	typedef void InterfaceType;

	TAttributeMember();

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
TAttributeMember<Attribute>::TAttributeMember()
:	m_attributePtr(nullptr),
	m_isAssigned(false)
{
}


template <typename Attribute>
void TAttributeMember<Attribute>::Init(
			const IComponent* ownerPtr,
			const IRealAttributeStaticInfo& staticInfo)
{
	InitInternal(ownerPtr, staticInfo, nullptr);
}

template <typename Attribute>
bool TAttributeMember<Attribute>::IsValid() const
{
	Q_ASSERT_X(m_isAssigned, "Component initialization", "No I_ASSIGN used or attribute is used out of component context");

	return (m_attributePtr != nullptr);
}


template <typename Attribute>
const Attribute* TAttributeMember<Attribute>::GetAttributePtr() const
{
	Q_ASSERT_X(m_isAssigned, "Component initialization", "No I_ASSIGN used or attribute is used out of component context");

	return m_attributePtr;
}


template <typename Attribute>
const typename TAttributeMember<Attribute>::AttributeValueType& TAttributeMember<Attribute>::GetOriginalValue() const
{
	Q_ASSERT(m_attributePtr != nullptr);	// GetOriginalValue() was called for invalid object, or no IsValid() check was called.

	return m_attributePtr->GetValue();
}


template <typename Attribute>
const Attribute* TAttributeMember<Attribute>::operator->() const
{
	Q_ASSERT_X(m_isAssigned, "Component initialization", "No I_ASSIGN used or attribute is used out of component context");

	return m_attributePtr;
}


template <typename Attribute>
const typename TAttributeMember<Attribute>::AttributeValueType& TAttributeMember<Attribute>::operator*() const
{
	Q_ASSERT(m_attributePtr != nullptr);	// operator* was called for invalid object, or no IsValid() check was called.

	return m_attributePtr->GetValue();
}


// protected methods

template <typename Attribute>
void TAttributeMember<Attribute>::SetAttribute(const Attribute* attributePtr)
{
	m_attributePtr = attributePtr;
}


template <typename Attribute>
bool TAttributeMember<Attribute>::InitInternal(
			const IComponent* ownerPtr,
			const IRealAttributeStaticInfo& staticInfo,
			const IComponent** definitionComponentPtr)
{
	Q_ASSERT(ownerPtr != nullptr);

	m_isAssigned = true;

	const QByteArray& attributeId = staticInfo.GetAttributeId();
	IComponentContextSharedPtr componentContextPtr = ownerPtr->GetComponentContext();
	if (componentContextPtr != nullptr){
		if (definitionComponentPtr != nullptr){
			int definitionLevel = -1;
			const iser::IObject* attributePtr = componentContextPtr->GetAttribute(attributeId, &definitionLevel);
			if (attributePtr != nullptr){
				m_attributePtr = dynamic_cast<const Attribute*>(attributePtr);

				if (m_attributePtr != nullptr){
					Q_ASSERT(definitionLevel >= 0);

					while (definitionLevel > 0 && (ownerPtr != nullptr)){
						ownerPtr = ownerPtr->GetParentComponent();
						// Q_ASSERT(ownerPtr != nullptr);

						--definitionLevel;
					}

					*definitionComponentPtr = ownerPtr;

					return true;
				}
				else{
					qCritical(	"Component '%s': Attribute '%s' type inconsistence!",
								CComponentContext::GetHierarchyAddress(componentContextPtr.get()).constData(),
								attributeId.constData());
				}
			}
		}
		else{
			const iser::IObject* attributePtr = componentContextPtr->GetAttribute(attributeId, nullptr);
			m_attributePtr = dynamic_cast<const Attribute*>(attributePtr);

			if (m_attributePtr == nullptr){
				if (attributePtr != nullptr){
					qCritical(	"Component %s: Attribute %s exists in the component context but has a wrong type",
								CComponentContext::GetHierarchyAddress(componentContextPtr.get()).constData(),
								attributeId.constData());
				}
			}

			return (m_attributePtr != nullptr);
		}
	}
	else{
		qCritical(	"Error during resolving of attribute: %s in component %s: Component context not set",
					CComponentContext::GetHierarchyAddress(componentContextPtr.get()).constData(),
					attributeId.constData());

		m_attributePtr = nullptr;
	}

	return false;
}


// Translatable attribute

class CTextAttribute: public iattr::CStringAttribute
{
public:
	typedef iattr::CStringAttribute BaseClass;

	CTextAttribute()
	{
	}

	explicit CTextAttribute(const QString& value)
	:	BaseClass(value)
	{
	}

	enum DefaultAttributeFlags
	{
		DAF_OBLIGATORY = iattr::CStringAttribute::DAF_OBLIGATORY | IAttributeStaticInfo::AF_TRANSLATABLE,
		DAF_OPTIONAL = iattr::CStringAttribute::DAF_OPTIONAL | IAttributeStaticInfo::AF_TRANSLATABLE
	};

	static QByteArray GetTypeName()
	{
		return "Text";
	}

	// reimplemented (iser::IObject)
	virtual QByteArray GetFactoryId() const
	{
		return GetTypeName();
	}
};


class CTextAttributeMember: public TAttributeMember<CTextAttribute>
{
public:
	typedef TAttributeMember<CTextAttribute> BaseClass;

	QString operator*() const
	{
		return QCoreApplication::translate("Attribute", BaseClass::operator*().toUtf8());
	}
};


} // namespace icomp


