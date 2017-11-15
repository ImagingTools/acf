#ifndef icomp_CSimComponentContextBase_included
#define icomp_CSimComponentContextBase_included


// Qt includes
#include <QtCore/QByteArray>
#include <QtCore/QString>
#include <QtCore/QMap>

// ACF includes
#include <istd/TIFactory.h>
#include <istd/CIdManipBase.h>
#include <icomp/IAttributeStaticInfo.h>
#include <iattr/TAttribute.h>
#include <iattr/TMultiAttribute.h>
#include <icomp/IComponent.h>
#include <icomp/IComponentContext.h>
#include <icomp/IComponentStaticInfo.h>
#include <icomp/TAttributeMember.h>
#include <icomp/TMultiAttributeMember.h>
#include <icomp/CRegistryElement.h>


namespace icomp
{


class CSimComponentContextBase:
			public IComponentContext,
			protected istd::CIdManipBase
{
public:
	typedef istd::TIFactory<icomp::IComponent> ComponentsFactory;

	explicit CSimComponentContextBase(const IComponentStaticInfo* infoPtr);

	/**
		Set named attribute.
		\param	attributeId		ID of attribute.
		\param	attributePtr	pointer to attribute instance. It will be automatically deleted.
	*/
	bool SetAttr(const QByteArray& attributeId, const iser::IObject* attributePtr);

	/**
		Set named reference to some component.
	*/
	bool SetRef(const QByteArray& referenceId, IComponent* componentPtr, const QByteArray& subelementId = "");

	/**
		Set named reference to some component.
	*/
	bool InsertMultiRef(const QByteArray& referenceId, IComponent* componentPtr, const QByteArray& subelementId = "");

	/**
		Set factory of component instance.
	*/
	bool SetFactory(const QByteArray& factoryId, const ComponentsFactory* factoryPtr);

	/**
		Insert new factory instance into multi-factory attribute.
	*/
	bool InsertMultiFactory(const QByteArray& factoryId, const ComponentsFactory* factoryPtr);

	/**
		Set instance of \c bool attribute.
	*/
	bool SetBoolAttr(const QByteArray& attributeId, bool value);

	/**
		Set instance of \c int attribute.
	*/
	bool SetIntAttr(const QByteArray& attributeId, int value);

	/**
		Set instance of \c double attribute.
	*/
	bool SetDoubleAttr(const QByteArray& attributeId, double value);

	/**
		Set instance of \c QString attribute.
	*/
	bool SetStringAttr(const QByteArray& attributeId, const QString& value);

	/**
		Set instance of \c QByteArray attribute.
	*/
	bool SetIdAttr(const QByteArray& attributeId, const QByteArray& value);

	/**
		Insert new attribute to multi attributes.
		\param	attributeId		ID of attribute (multi attribute).
		\param	attribute		single attribute value.
	*/
	template <class Attribute>
	bool InsertMultiAttr(const QByteArray& attributeId, const Attribute& attribute)
	{
		Q_ASSERT(IsAttributeTypeCorrect<iattr::TMultiAttribute<Attribute> >(attributeId));

		iattr::TMultiAttribute<Attribute>* multiAttrPtr = NULL;

		const IRegistryElement::AttributeInfo* existingInfoPtr = m_registryElement.GetAttributeInfo(attributeId);
		if (existingInfoPtr != NULL){
			multiAttrPtr = dynamic_cast<iattr::TMultiAttribute<Attribute>*>(existingInfoPtr->attributePtr.GetPtr());
		}
		else{
			IRegistryElement::AttributeInfo* newInfoPtr = m_registryElement.InsertAttributeInfo(attributeId, istd::CClassInfo::GetName<iattr::TMultiAttribute<Attribute> >());
			if (newInfoPtr != NULL){
				IRegistryElement::AttributePtr& attributePtr = newInfoPtr->attributePtr;
				if (!attributePtr.IsValid()){
					attributePtr.SetPtr(new iattr::TMultiAttribute<Attribute>);
				}

				multiAttrPtr = dynamic_cast<iattr::TMultiAttribute<Attribute>*>(attributePtr.GetPtr());
			}
		}

		if (multiAttrPtr != NULL){
			multiAttrPtr->InsertValue(attribute);

			return true;
		}

		return false;
	}

	bool InsertMultiAttr(const QByteArray& attributeId, const QString& attribute);

	// reimplemeted (icomp::IComponentContext)
	virtual const IRegistryElement& GetRegistryElement() const;
	virtual const IComponentStaticInfo& GetStaticInfo() const;
	virtual const IComponentContext* GetParentContext() const;
	virtual const iser::IObject* GetAttribute(const QByteArray& attributeId, int* definitionLevelPtr = NULL) const;

protected:
	/**
		Check if attribute type is corrected.
	*/
	template <class AttrType>
	bool IsAttributeTypeCorrect(const QByteArray& attributeId);

	typedef QMap<QByteArray, IComponent*> ComponentsMap;
	ComponentsMap m_componentsMap;

	typedef QMap< QByteArray, const ComponentsFactory* > FactoriesMap;
	FactoriesMap m_factoriesMap;

private:
	CRegistryElement m_registryElement;
	const IComponentStaticInfo& m_metaInfo;
};


// protected methods

template <class AttrType>
bool CSimComponentContextBase::IsAttributeTypeCorrect(const QByteArray& attributeId)
{
	const IAttributeStaticInfo* attributeInfoPtr = m_metaInfo.GetAttributeInfo(attributeId);
	if (attributeInfoPtr != NULL){
		QByteArray attributeType = attributeInfoPtr->GetAttributeTypeId();

		return attributeType == AttrType::GetTypeName();
	}

	return false;
}


} // namespace icomp


#endif // !icomp_CSimComponentContextBase_included


