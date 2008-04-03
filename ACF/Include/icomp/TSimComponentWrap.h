#ifndef icomp_TSimComponentWrap_included
#define icomp_TSimComponentWrap_included


#include <map>

#include "icomp/IComponent.h"
#include "icomp/IComponentContext.h"
#include "icomp/TComponentWrap.h"
#include "icomp/CRegistryElement.h"


namespace icomp
{


/**
	Simulation wrapper of component.
	It allows to use components directly from static linked libraries, without component framework.
*/
template <class Base>
class TSimComponentWrap: public TComponentWrap<Base>, protected CRegistryElement, protected IComponentContext
{
public:
	typedef TComponentWrap<Base> BaseClass;

	TSimComponentWrap();

	/**
		Initialilze component after setting all its attributes and references.
	*/
	void InitComponent();

	/**
		Set named reference to some component.
	*/
	bool SetRef(const ::std::string& referenceId, IComponent* componentPtr);

	/**
		Set named attribute.
		\param	attributeId		ID of attribute.
		\param	attributePtr	pointer to attribute instance. It will be automatically deleted.
	*/
	bool SetAttr(const ::std::string& attributeId, iser::ISerializable* attributePtr);

	/**
		Set instance of simple attribute.
	*/
	template <class Attribute>
	bool SetSimpleAttr(const ::std::string& attributeId, const Attribute& attribute)
	{
		return SetAttr(attributeId, new TSimpleAttribute<Attribute>(attribute));
	}

protected:
	// reimplemeted (icomp::IComponentContext)
	virtual const IRegistryElement& GetRegistryElement() const;
	virtual const IComponentContext* GetParentContext() const;
	virtual const iser::ISerializable* GetAttribute(const ::std::string& attributeId, const IComponentContext** realContextPtr = NULL) const;
	virtual IComponent* GetSubcomponent(const ::std::string& componentId) const;

private:
	typedef ::std::map< ::std::string, IComponent*> ComponentMap;
	ComponentMap m_componentMap;
};


// public methods

template <class Base>
TSimComponentWrap<Base>::TSimComponentWrap()
:	CRegistryElement(&InitStaticInfo(this))
{
}


template <class Base>
void TSimComponentWrap<Base>::InitComponent()
{
	SetComponentContext(this);
}


template <class Base>
bool TSimComponentWrap<Base>::SetRef(const ::std::string& referenceId, IComponent* componentPtr)
{
	I_ASSERT(componentPtr != NULL);

	AttributeInfo* infoPtr = InsertAttributeInfo(referenceId, false);
	if (infoPtr != NULL){
		infoPtr->attributePtr.SetPtr(new CReferenceAttribute(referenceId));

		m_componentMap[referenceId] = componentPtr;

		return true;
	}

	return false;
}


template <class Base>
bool TSimComponentWrap<Base>::SetAttr(const ::std::string& attributeId, iser::ISerializable* attributePtr)
{
	I_ASSERT(attributePtr != NULL);

	AttributeInfo* infoPtr = InsertAttributeInfo(attributeId, false);
	if (infoPtr != NULL){
		infoPtr->attributePtr.SetPtr(attributePtr);

		return true;
	}

	return false;
}


// protected methods

// reimplemeted (icomp::IComponentContext)

template <class Base>
const IRegistryElement& TSimComponentWrap<Base>::GetRegistryElement() const
{
	return *this;
}


template <class Base>
const IComponentContext* TSimComponentWrap<Base>::GetParentContext() const
{
	return this;
}


template <class Base>
const iser::ISerializable* TSimComponentWrap<Base>::GetAttribute(const ::std::string& attributeId, const IComponentContext** realContextPtr) const
{
	const AttributeInfo* infoPtr = GetAttributeInfo(attributeId);
	if (infoPtr != NULL){
		if (realContextPtr != NULL){
			*realContextPtr = this;
		}
		return infoPtr->attributePtr.GetPtr();
	}

	const IComponentStaticInfo& componentInfo = GetComponentStaticInfo();
	const IComponentStaticInfo::AttributeInfos& attributeInfos = componentInfo.GetAttributeInfos();
	const IComponentStaticInfo::AttributeInfos::ValueType* attributePtr2 = attributeInfos.FindElement(attributeId);
	if (attributePtr2 != NULL){
		I_ASSERT(*attributePtr2 != NULL);

		if ((*attributePtr2)->IsObligatory()){
			const iser::ISerializable* defaultAttributePtr = (*attributePtr2)->GetAttributeDefaultValue();
			if (defaultAttributePtr != NULL){
				if (realContextPtr != NULL){
					*realContextPtr = this;
				}

				return defaultAttributePtr;
			}
		}
	}

	return NULL;
}


template <class Base>
IComponent* TSimComponentWrap<Base>::GetSubcomponent(const ::std::string& componentId) const
{
	ComponentMap::const_iterator iter = m_componentMap.find(componentId);

	if (iter != m_componentMap.end()){
		return iter->second;
	}

	return NULL;
}


}//namespace icomp


#endif //!icomp_TSimComponentWrap_included


