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
		Set named reference to some component.
	*/
	bool SetRef(const ::std::string& referenceId, IComponent* componentPtr);
	/**
		Set named attribute.
	*/
	bool SetAttr(const ::std::string& attributeId, iser::ISerializable* attributePtr);

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
:	BaseClass(this),
	CRegistryElement(&InitStaticInfo(this))
{
}


template <class Base>
bool TSimComponentWrap<Base>::SetRef(const ::std::string& referenceId, IComponent* componentPtr)
{
	I_ASSERT(attributePtr != NULL);

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


