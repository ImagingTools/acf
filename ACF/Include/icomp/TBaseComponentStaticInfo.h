#ifndef icomp_TBaseComponentStaticInfo_included
#define icomp_TBaseComponentStaticInfo_included


#include "icomp/TComponentWrap.h"
#include "icomp/CComponentStaticInfoBase.h"


namespace icomp
{


/**
	Standard implementation of static info for base component classes.
	The main difference to 'normal' component static info is, that instances of such components cannot be created.
*/
template <class Component>
class TBaseComponentStaticInfo: virtual public CComponentStaticInfoBase
{
public:
	typedef CComponentStaticInfoBase BaseClass;

	TBaseComponentStaticInfo(const IComponentStaticInfo* baseComponentPtr = NULL);

	//	reimplemented (icomp::IComponentStaticInfo)
	virtual IComponent* CreateComponent(const IComponentContext* contextPtr) const;
	virtual const InterfaceExtractors& GetInterfaceExtractors() const;
	virtual const AttributeInfos& GetAttributeInfos() const;
	virtual const IComponentStaticInfo* GetSubcomponent(const std::string& subcomponentId) const;
	virtual bool RegisterInterfaceExtractor(const std::string& interfaceId, InterfaceExtractorPtr extractorPtr);
	virtual bool RegisterAttributeInfo(const std::string& attributeId, const IAttributeStaticInfo* attributeInfoPtr);

private:
	const IComponentStaticInfo* m_baseComponentPtr;

	InterfaceExtractors m_interfaceExtractors;
	AttributeInfos m_attributeInfos;
};


// public methods

template <class Component>
TBaseComponentStaticInfo<Component>::TBaseComponentStaticInfo(const IComponentStaticInfo* baseComponentPtr)
:	m_baseComponentPtr(baseComponentPtr)
{
	if (baseComponentPtr != NULL){
		m_interfaceExtractors.SetParent(&baseComponentPtr->GetInterfaceExtractors());
		m_attributeInfos.SetParent(&baseComponentPtr->GetAttributeInfos());
	}
}


//	reimplemented (icomp::IComponentStaticInfo)

template <class Component>
IComponent* TBaseComponentStaticInfo<Component>::CreateComponent(const IComponentContext* /*contextPtr*/) const
{
	I_CRITICAL();	// trying to create abstract base component.

	return NULL;
}


template <class Component>
const IComponentStaticInfo::InterfaceExtractors& TBaseComponentStaticInfo<Component>::GetInterfaceExtractors() const
{
	return m_interfaceExtractors;
}


template <class Component>
const IComponentStaticInfo::AttributeInfos& TBaseComponentStaticInfo<Component>::GetAttributeInfos() const
{
	return m_attributeInfos;
}


template <class Component>
const IComponentStaticInfo* TBaseComponentStaticInfo<Component>::GetSubcomponent(const std::string& subcomponentId) const
{
	const IComponentStaticInfo* retVal = BaseClass::GetSubcomponent(subcomponentId);
	if (retVal != NULL){
		return retVal;
	}
	else if (m_baseComponentPtr != NULL){
		return m_baseComponentPtr->GetSubcomponent(subcomponentId);
	}
	else{
		return NULL;
	}
}


template <class Component>
bool TBaseComponentStaticInfo<Component>::RegisterInterfaceExtractor(const std::string& interfaceId, InterfaceExtractorPtr extractorPtr)
{
	return m_interfaceExtractors.InsertLocal(interfaceId, extractorPtr);
}


template <class Component>
bool TBaseComponentStaticInfo<Component>::RegisterAttributeInfo(const std::string& attributeId, const IAttributeStaticInfo* attributeInfoPtr)
{
	return m_attributeInfos.InsertLocal(attributeId, attributeInfoPtr);
}


// public methods of embedded class InterfaceRegistrator

template <class Component>
TBaseComponentStaticInfo<Component>::InterfaceRegistrator::InterfaceRegistrator(icomp::IComponentStaticInfo* staticInfoPtr)
{
	if (staticInfoPtr != NULL){
		staticInfoPtr->RegisterInterfaceExtractor(typeid(Component).name());
	}
}


}//namespace icomp


#endif // !icomp_TBaseComponentStaticInfo_included


