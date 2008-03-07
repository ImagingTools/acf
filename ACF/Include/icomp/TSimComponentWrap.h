#ifndef icomp_TSimComponentWrap_included
#define icomp_TSimComponentWrap_included


template <class Base>
class TSimComponentWrap: public Base, protected CRegistryElement, protected IComponentContext
{
public:
	typedef Base BaseClass;

	TSimComponentWrap();

	bool SetRef(const ::std::string& referenceId, IComponent* componentPtr);
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
	CRegistryElement(&GetStaticInfo())
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

const IRegistryElement& TSimComponentWrap<Base>::GetRegistryElement() const
{
	return this;
}


const IComponentContext* TSimComponentWrap<Base>::GetParentContext() const
{
	return this;
}


const iser::ISerializable* TSimComponentWrap<Base>::GetAttribute(const ::std::string& attributeId, const IComponentContext** realContextPtr = NULL) const
{
	AttributeInfo* infoPtr = GetAttributeInfo(attributeId);
	if (infoPtr != NULL){
		return infoPtr->attributePtr.GetPtr();
	}

	return NULL;
}


IComponent* TSimComponentWrap<Base>::GetSubcomponent(const ::std::string& componentId) const
{
	ComponentMap::iterator iter = m_componentMap.find();

	if (iter != m_componentMap.end()){
		return iter->second;
	}

	return NULL;
}


#endif //!icomp_TSimComponentWrap_included


