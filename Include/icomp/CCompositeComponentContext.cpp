#include "icomp/CCompositeComponentContext.h"


#include "icomp/IRegistry.h"
#include "icomp/IRegistriesManager.h"


namespace icomp
{


CCompositeComponentContext::CCompositeComponentContext(
			const IRegistryElement* elementPtr,
			const IComponentStaticInfo* staticInfoPtr,
			const IRegistry* registryPtr,
			const IComponentEnvironmentManager* environmentManagerPtr,
			const CCompositeComponentContext* parentPtr,
			const QByteArray& contextId)
:	BaseClass(elementPtr, staticInfoPtr, parentPtr, contextId),
	m_registry(*registryPtr),
	m_environmentManager(*environmentManagerPtr)
{
	Q_ASSERT(registryPtr != NULL);
	Q_ASSERT(environmentManagerPtr != NULL);
}


} // namespace icomp


