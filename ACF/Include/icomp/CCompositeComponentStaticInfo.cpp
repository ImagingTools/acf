#include "icomp/CCompositeComponentStaticInfo.h"


#include "icomp/TComponentWrap.h"
#include "icomp/CCompositeComponent.h"


namespace icomp
{


CCompositeComponentStaticInfo::CCompositeComponentStaticInfo(const IRegistry* m_registryPtr)
:	m_registry(*m_registryPtr)
{
	I_ASSERT(m_registryPtr != NULL);
}


//	reimplemented (icomp::IComponentStaticInfo)

IComponent* CCompositeComponentStaticInfo::CreateComponent(const IComponentContext* contextPtr) const
{
	TComponentWrap<CCompositeComponent>* componentPtr = new TComponentWrap<CCompositeComponent>(contextPtr);

	return componentPtr;
}


const IComponentStaticInfo::InterfaceExtractors& CCompositeComponentStaticInfo::GetInterfaceExtractors() const
{
	static InterfaceExtractors empty;

	// TODO: implement GetInterfaceExtractors() from exported interfaces for CCompositeComponentStaticInfo

	return empty;
}


const IComponentStaticInfo::AttributeInfos& CCompositeComponentStaticInfo::GetAttributeInfos() const
{
	static AttributeInfos empty;

	// TODO: implement GetAttributeInfos() from exported interfaces for CCompositeComponentStaticInfo

	return empty;
}


IComponentStaticInfo::Ids CCompositeComponentStaticInfo::GetSubcomponentIds() const
{
	static Ids retVal;

	return retVal;
}


const IComponentStaticInfo* CCompositeComponentStaticInfo::GetSubcomponent(const std::string& /*subcomponentId*/) const
{
	return NULL;
}


bool CCompositeComponentStaticInfo::RegisterInterfaceExtractor(const std::string& /*interfaceId*/, InterfaceExtractorPtr /*extractorPtr*/)
{
	return false;
}


bool CCompositeComponentStaticInfo::RegisterAttributeInfo(const std::string& /*attributeId*/, const IAttributeStaticInfo* /*attributeInfoPtr*/)
{
	return false;
}


bool CCompositeComponentStaticInfo::RegisterSubcomponentInfo(const std::string& /*subcomponentId*/, const IComponentStaticInfo* /*componentInfoPtr*/)
{
	return false;
}


istd::CString CCompositeComponentStaticInfo::GetDescription() const
{
	return "";
}


istd::CString CCompositeComponentStaticInfo::GetKeywords() const
{
	return "";
}


}//namespace icomp


