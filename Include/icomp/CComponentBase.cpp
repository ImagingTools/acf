#include "icomp/CComponentBase.h"


#include "icomp/IComponentContext.h"
#include "icomp/IRegistryElement.h"
#include "icomp/CBaseComponentStaticInfo.h"


namespace icomp
{


CComponentBase::CComponentBase()
:	m_contextPtr(NULL), m_parentPtr(NULL), m_isParentOwner(false)
{
}


// reimplemented (icomp::IComponent)

const ICompositeComponent* CComponentBase::GetParentComponent(bool ownerOnly) const
{
	if (!ownerOnly || m_isParentOwner){
		return m_parentPtr;
	}

	return NULL;
}


void* CComponentBase::GetInterface(const istd::CClassInfo& interfaceType, const QByteArray& subId)
{
	if (m_contextPtr != NULL){
		const IRealComponentStaticInfo& staticInfo = GetComponentStaticInfo();

		return staticInfo.GetComponentInterface(interfaceType, *this, subId);
	}

	return NULL;
}


const IComponentContext* CComponentBase::GetComponentContext() const
{
	return m_contextPtr;
}


void CComponentBase::SetComponentContext(
			const icomp::IComponentContext* contextPtr,
			const ICompositeComponent* parentPtr,
			bool isParentOwner)
{
	m_contextPtr = contextPtr;
	m_parentPtr = parentPtr;
	m_isParentOwner = isParentOwner;
}


// protected methods

void CComponentBase::OnComponentCreated()
{
	Q_ASSERT(m_contextPtr != NULL);
}


void CComponentBase::OnComponentDestroyed()
{
	Q_ASSERT(m_contextPtr != NULL);
}


// static protected methods

const icomp::IRealComponentStaticInfo& CComponentBase::InitStaticInfo(IComponent* componentPtr)
{
	static icomp::CBaseComponentStaticInfo staticInfo(NULL);
	static bool isStaticInitialized = false;
	if (isStaticInitialized && (componentPtr == NULL)){
		return staticInfo;
	}

	isStaticInitialized = true;

	I_REGISTER_INTERFACE(istd::IPolymorphic);

	return staticInfo;
}


} // namespace icomp


