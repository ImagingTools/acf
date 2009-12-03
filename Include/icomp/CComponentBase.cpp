#include "icomp/CComponentBase.h"


#include "icomp/IComponentContext.h"
#include "icomp/IRegistryElement.h"
#include "icomp/CPackageStaticInfo.h"


namespace icomp
{


CComponentBase::CComponentBase()
:	m_contextPtr(NULL), m_parentPtr(NULL), m_isParentOwner(false)
{
}


// reimplemented (icomp::IComponent)

const IComponent* CComponentBase::GetParentComponent(bool ownerOnly) const
{
	if (!ownerOnly || m_isParentOwner){
		return m_parentPtr;
	}

	return NULL;
}


void* CComponentBase::GetInterface(const istd::CClassInfo& interfaceType, const std::string& /*subId*/)
{
	if (!interfaceType.IsValid()){
		return this;
	}

	if (m_contextPtr != NULL){
		const IRegistryElement& registryElement = m_contextPtr->GetRegistryElement();

		const IComponentStaticInfo& staticInfo = registryElement.GetComponentStaticInfo();

		const IComponentStaticInfo::InterfaceExtractors& extractors = staticInfo.GetInterfaceExtractors();

		int index = extractors.FindIndex(interfaceType);

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


void CComponentBase::SetComponentContext(
			const icomp::IComponentContext* contextPtr,
			const IComponent* parentPtr,
			bool isParentOwner)
{
	m_contextPtr = contextPtr;
	m_parentPtr = parentPtr;
	m_isParentOwner = isParentOwner;
}


IComponent* CComponentBase::GetSubcomponent(const std::string& /*componentId*/) const
{
	return NULL;	// normal component has no subcomponents
}


const IComponentContext* CComponentBase::GetSubcomponentContext(const std::string& /*componentId*/) const
{
	return NULL;	// normal component has no subcomponents and also their contexts
}


IComponent* CComponentBase::CreateSubcomponent(const std::string& /*componentId*/) const
{
	return NULL;	// normal component cannot create subcomponents
}


void CComponentBase::OnSubcomponentDeleted(const IComponent* /*subcomponentPtr*/)
{
	// do nothing, real components has no subcomponents
}


// static methods

const icomp::IComponentStaticInfo& CComponentBase::InitStaticInfo(CComponentBase* /*componentPtr*/)
{
	static CPackageStaticInfo emptyInfo;

	return emptyInfo;
}


// protected methods

// reimplemented (icomp::IComponent)

void CComponentBase::OnComponentCreated()
{
	I_ASSERT(m_contextPtr != NULL);
}


void CComponentBase::OnComponentDestroyed()
{
	I_ASSERT(m_contextPtr != NULL);
}


}//namespace icomp


