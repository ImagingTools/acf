#include "icomp/CComponentBase.h"


#include "icomp/IComponentContext.h"
#include "icomp/IRegistryElement.h"
#include "icomp/IComponentStaticInfo.h"


namespace icomp
{


CComponentBase::CComponentBase(const IComponentContext* contextPtr)
:	m_context(*contextPtr)
{
	I_ASSERT(contextPtr != NULL);
}


// reimplemented (icomp::IComponent)

void* CComponentBase::GetInterface(const type_info& interfaceType, const ::std::string& /*subId*/)
{
	const IRegistryElement& registryElement = m_context.GetRegistryElement();

	const IComponentStaticInfo& staticInfo = registryElement.GetComponentStaticInfo();

	const IComponentStaticInfo::InterfaceExtractors& extractors = staticInfo.GetInterfaceExtractors();

	int index = extractors.FindIndex(interfaceType.name());

	if (index > 0){
		IComponentStaticInfo::InterfaceExtractorPtr extractorPtr = extractors.GetValueAt(index);

		return extractorPtr(this);
	}

	return NULL;
}


const IComponentContext& CComponentBase::GetComponentContext() const
{
	return m_context;
}


}//namespace icomp


