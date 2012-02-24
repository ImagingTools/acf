#ifndef icomp_CSimComponentContextBase_included
#define icomp_CSimComponentContextBase_included


// STL includes
#include <map>

#include <QString>

#include "istd/TIFactory.h"
#include "istd/CIdManipBase.h"

#include "icomp/IComponent.h"
#include "icomp/IComponentContext.h"
#include "icomp/IComponentStaticInfo.h"
#include "icomp/TAttribute.h"
#include "icomp/TMultiAttribute.h"
#include "icomp/CRegistryElement.h"


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
	bool SetAttr(const std::string& attributeId, const iser::IObject* attributePtr);

	/**
		Set named reference to some component.
	*/
	bool SetRef(const std::string& referenceId, IComponent* componentPtr, const std::string& subelementId = "");

	/**
		Set named reference to some component.
	*/
	bool InsertMultiRef(const std::string& referenceId, IComponent* componentPtr, const std::string& subelementId = "");

	/**
		Set factory of component instance.
	*/
	bool SetFactory(const std::string& factoryId, const ComponentsFactory* factoryPtr);

	/**
		Set instance of \c bool attribute.
	*/
	bool SetBoolAttr(const std::string& attributeId, bool value);

	/**
		Set instance of \c int attribute.
	*/
	bool SetIntAttr(const std::string& attributeId, int value);

	/**
		Set instance of \c double attribute.
	*/
	bool SetDoubleAttr(const std::string& attributeId, double value);

	/**
		Set instance of \c QString attribute.
	*/
	bool SetStringAttr(const std::string& attributeId, const QString& value);

	/**
		Set instance of simple attribute.
	*/
	template <class Attribute>
	bool SetSingleAttr(const std::string& attributeId, const Attribute& attribute)
	{
		I_ASSERT(IsAttributeTypeCorrect<TAttribute<Attribute> >(attributeId));

		return SetAttr(attributeId, new TAttribute<Attribute>(attribute));
	}

	/**
		Insert new attribute to multi attributes.
		\param	attributeId		ID of attribute (multi attribute).
		\param	attribute		single attribute value.
	*/
	template <class Attribute>
	bool InsertMultiAttr(const std::string& attributeId, const Attribute& attribute)
	{
		I_ASSERT(IsAttributeTypeCorrect<TMultiAttribute<Attribute> >(attributeId));

		TMultiAttribute<Attribute>* multiAttrPtr = NULL;

		const IRegistryElement::AttributeInfo* existingInfoPtr = m_registryElement.GetAttributeInfo(attributeId);
		if (existingInfoPtr != NULL){
			multiAttrPtr = dynamic_cast<TMultiAttribute<Attribute>*>(existingInfoPtr->attributePtr.GetPtr());
		}
		else{
			IRegistryElement::AttributeInfo* newInfoPtr = m_registryElement.InsertAttributeInfo(attributeId, istd::CClassInfo::GetName<TMultiAttribute<Attribute> >());
			if (newInfoPtr != NULL){
				IRegistryElement::AttributePtr& attributePtr = newInfoPtr->attributePtr;
				if (!attributePtr.IsValid()){
					attributePtr.SetPtr(new TMultiAttribute<Attribute>);
				}

				multiAttrPtr = dynamic_cast<TMultiAttribute<Attribute>*>(attributePtr.GetPtr());
			}
		}

		if (multiAttrPtr != NULL){
			multiAttrPtr->InsertValue(attribute);

			return true;
		}

		return false;
	}

	// reimplemeted (icomp::IComponentContext)
	virtual const IRegistryElement& GetRegistryElement() const;
	virtual const IComponentStaticInfo& GetStaticInfo() const;
	virtual const IComponentContext* GetParentContext() const;
	virtual const iser::IObject* GetAttribute(const std::string& attributeId, int* definitionLevelPtr = NULL) const;

protected:
	/**
		Check if attribute type is corrected.
	*/
	template <class AttrType>
	bool IsAttributeTypeCorrect(const std::string& attributeId);

	typedef std::map<std::string, IComponent*> ComponentsMap;
	ComponentsMap m_componentsMap;

	typedef std::map< std::string, const ComponentsFactory* > FactoriesMap;
	FactoriesMap m_factoriesMap;

private:
	CRegistryElement m_registryElement;
	const IComponentStaticInfo& m_metaInfo;
};


// protected methods

template <class AttrType>
bool CSimComponentContextBase::IsAttributeTypeCorrect(const std::string& attributeId)
{
	const IAttributeStaticInfo* attributeInfoPtr = m_metaInfo.GetAttributeInfo(attributeId);
	if (attributeInfoPtr != NULL){
		std::string attributeType = attributeInfoPtr->GetAttributeTypeName();

		return attributeType == AttrType::GetTypeName();
	}

	return false;
}


} // namespace icomp


#endif // !icomp_CSimComponentContextBase_included


