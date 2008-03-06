#ifndef icomp_TComponentStaticInfo_included
#define icomp_TComponentStaticInfo_included


#include "icomp/IComponentStaticInfo.h"


namespace icomp
{


template <class Component>
class TComponentStaticInfo: virtual public IComponentStaticInfo
{
public:
	TComponentStaticInfo(IComponentStaticInfo* baseComponentPtr = NULL);

	//	reimplemented (icomp::IComponentStaticInfo)
	virtual IComponent* CreateComponent(const IComponentContext* contextPtr) const;
	virtual const InterfaceExtractors& GetInterfaceExtractors() const;
	virtual const AttributeInfos& GetAttributeInfos() const;
	virtual const SubcomponentInfos& GetSubcomponentInfos() const;
	virtual bool RegisterInterfaceExtractor(const ::std::string& interfaceId, InterfaceExtractorPtr extractorPtr);
	virtual bool RegisterAttributeInfo(const ::std::string& attributeId, const IAttributeStaticInfo* attributeInfoPtr);
	virtual bool RegisterSubcomponentInfo(const ::std::string& subcomponentId, const IComponentStaticInfo* componentInfoPtr);

private:
	IComponentStaticInfo* m_baseComponentPtr;

	InterfaceExtractors m_interfaceExtractors;
	AttributeInfos m_attributeInfos;
	SubcomponentInfos m_subcomponentInfos;
};


// public methods

template <class Component>
TComponentStaticInfo<Component>::TComponentStaticInfo(IComponentStaticInfo* baseComponentPtr)
:	m_baseComponentPtr(baseComponentPtr),
{
	if (baseComponentPtr != NULL){
		m_interfaceExtractors.SetParent(baseComponentPtr->m_interfaceExtractors);
		m_attributeInfos.SetParent(baseComponentPtr->m_attributeInfos);
		m_subcomponentInfos.SetParent(baseComponentPtr->m_subcomponentInfos);
	}
}


//	reimplemented (icomp::IComponentStaticInfo)

template <class Component>
IComponent* TComponentStaticInfo<Component>::CreateComponent(const IComponentContext* contextPtr) const
{
	return new Component(contextPtr);
}


template <class Component>
const IComponentStaticInfo::InterfaceExtractors& TComponentStaticInfo<Component>::GetInterfaceExtractors() const
{
	return m_interfaceExtractors;
}


template <class Component>
const IComponentStaticInfo::AttributeInfos& TComponentStaticInfo<Component>::GetAttributeInfos() const
{
	return m_attributeInfos;
}


template <class Component>
const IComponentStaticInfo::SubcomponentInfos& TComponentStaticInfo<Component>::GetSubcomponentInfos() const
{
	return m_subcomponentInfos;
}


template <class Component>
bool TComponentStaticInfo<Component>::RegisterInterfaceExtractor(const ::std::string& interfaceId, InterfaceExtractorPtr extractorPtr)
{
	return m_interfaceExtractors.InsertLocal(interfaceId, extractorPtr);
}


template <class Component>
bool TComponentStaticInfo<Component>::RegisterAttributeInfo(const ::std::string& attributeId, const IAttributeStaticInfo* attributeInfoPtr)
{
	return m_attributeInfos.InsertLocal(attributeId, attributeInfoPtr);
}


template <class Component>
bool TComponentStaticInfo<Component>::RegisterSubcomponentInfo(const ::std::string& subcomponentId, const IComponentStaticInfo* componentInfoPtr)
{
	return m_subcomponentInfos.InsertLocal(subcomponentId, componentInfoPtr);
}


// public methods of embedded class InterfaceRegistrator

template <class Component>
TComponentStaticInfo<Component>::InterfaceRegistrator::InterfaceRegistrator(icomp::IComponentStaticInfo* staticInfoPtr)
{
	if (staticInfoPtr != NULL){
		staticInfoPtr->RegisterInterfaceExtractor(typeid(Component).name());
	}
}


}//namespace icomp


#endif // !icomp_TComponentStaticInfo_included


