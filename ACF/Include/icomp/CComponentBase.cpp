#include "icomp/CComponentBase.h"


#include "icomp/IComponentContext.h"
#include "icomp/IRegistryElement.h"
#include "icomp/CPackageStaticInfo.h"


namespace icomp
{


CComponentBase::CComponentBase()
:	m_contextPtr(NULL)
{
}


// reimplemented (icomp::IComponent)

void* CComponentBase::GetInterface(const type_info& interfaceType, const std::string& /*subId*/)
{
	if (m_contextPtr != NULL){
		const IRegistryElement& registryElement = m_contextPtr->GetRegistryElement();

		const IComponentStaticInfo& staticInfo = registryElement.GetComponentStaticInfo();

		const IComponentStaticInfo::InterfaceExtractors& extractors = staticInfo.GetInterfaceExtractors();

		int index = extractors.FindIndex(interfaceType.name());

		if (index >= 0){
			IComponentStaticInfo::InterfaceExtractorPtr extractorPtr = extractors.GetValueAt(index);

			return extractorPtr(this);
		}
	}

	return NULL;
}


const IComponentContext* CComponentBase::GetComponentContext() const
{
	return m_contextPtr;
}


void CComponentBase::OnComponentCreated()
{
	I_ASSERT(m_contextPtr != NULL);
}


void CComponentBase::OnComponentDestroyed()
{
	I_ASSERT(m_contextPtr != NULL);
}


void CComponentBase::SetComponentContext(const icomp::IComponentContext* contextPtr)
{
	m_contextPtr = contextPtr;
}


// static methods

const icomp::IComponentStaticInfo& CComponentBase::InitStaticInfo(CComponentBase* /*componentPtr*/)
{
	static CPackageStaticInfo emptyInfo;

	return emptyInfo;
}


}//namespace icomp


