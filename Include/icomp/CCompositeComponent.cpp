#include <icomp/CCompositeComponent.h>


#ifndef QT_NO_DEBUG
// Qt includes
#include <QtCore/QThread>
#include <QtCore/QCoreApplication>
#endif //QT_NO_DEBUG


// ACF includes
#include <istd/CClassInfo.h>
#include <icomp/IComponentEnvironmentManager.h>
#include <icomp/CBaseComponentStaticInfo.h>


namespace icomp
{


CCompositeComponent::CCompositeComponent(bool manualAutoInit)
:	m_contextPtr(NULL),
	m_parentPtr(NULL),
	m_isParentOwner(false),
	m_autoInitialized(false),
	m_manualAutoInit(manualAutoInit)
{
}


CCompositeComponent::~CCompositeComponent()
{
	if (m_isParentOwner && (m_parentPtr != NULL)){
		const_cast<ICompositeComponent*>(m_parentPtr)->OnSubcomponentDeleted(this);
	}

	SetComponentContext(NULL, NULL, false);
}


bool CCompositeComponent::EnsureAutoInitComponentsCreated() const
{
	bool retVal = false;

	if ((m_contextPtr != NULL) && !m_autoInitialized){
		m_autoInitialized = true;

		while (!m_autoInitComponentIds.isEmpty()){
			QByteArray autoInitId = *m_autoInitComponentIds.begin();

			m_autoInitComponentIds.erase(m_autoInitComponentIds.begin());

			ComponentInfo& autoInitInfo = m_componentMap[autoInitId];
			if (!autoInitInfo.isComponentInitialized){
				autoInitInfo.isComponentInitialized = true;
				autoInitInfo.isContextInitialized = true;

				CreateSubcomponentInfo(autoInitId, autoInitInfo.contextPtr, &autoInitInfo.componentPtr, true);

				retVal = true;
			}
		}

		for (		ComponentMap::iterator iter = m_componentMap.begin();
					iter != m_componentMap.end();
					++iter){
			ComponentInfo& info = iter.value();

			icomp::CCompositeComponent* compositeComponentPtr = dynamic_cast<icomp::CCompositeComponent*>(info.componentPtr.GetPtr());
			if (compositeComponentPtr != NULL){
				retVal = compositeComponentPtr->EnsureAutoInitComponentsCreated() || retVal;
			}
		}
	}

	return retVal;
}


// reimplemented (icomp::ICompositeComponent)

IComponent* CCompositeComponent::GetSubcomponent(const QByteArray& componentId) const
{
	if (m_contextPtr != NULL){
		ComponentInfo& componentInfo = m_componentMap[componentId];
		if (!componentInfo.isComponentInitialized){
#ifndef QT_NO_DEBUG
			if ((qApp != NULL) && QThread::currentThread() != qApp->thread()){
				qWarning("Component %s initialized outside of an application thread\n", m_contextPtr->GetCompleteContextId().constData());
			}
#endif //QT_NO_DEBUG

			componentInfo.isComponentInitialized = true;
			componentInfo.isContextInitialized = true;

			CreateSubcomponentInfo(componentId, componentInfo.contextPtr, &componentInfo.componentPtr, true);
		}

		return componentInfo.componentPtr.GetPtr();
	}
	else{
		ComponentMap::ConstIterator iter = m_componentMap.constFind(componentId);
		if (iter != m_componentMap.constEnd()){
			return iter.value().componentPtr.GetPtr();
		}
		else{
			return NULL;
		}
	}
}


const IComponentContext* CCompositeComponent::GetSubcomponentContext(const QByteArray& componentId) const
{
	if (m_contextPtr != NULL){
		ComponentInfo& componentInfo = m_componentMap[componentId];
		if (!componentInfo.isContextInitialized){
			componentInfo.isContextInitialized = true;

			CreateSubcomponentInfo(componentId, componentInfo.contextPtr, NULL, true);
		}

		return componentInfo.contextPtr.GetPtr();
	}
	else{
		ComponentMap::ConstIterator iter = m_componentMap.constFind(componentId);
		if (iter != m_componentMap.constEnd()){
			return iter.value().contextPtr.GetPtr();
		}
		else{
			return NULL;
		}
	}
}


IComponent* CCompositeComponent::CreateSubcomponent(const QByteArray& componentId) const
{
	if (m_contextPtr != NULL){
		ComponentPtr retVal;

		ComponentInfo& componentInfo = m_componentMap[componentId];

		componentInfo.isContextInitialized = true;
		CreateSubcomponentInfo(componentId, componentInfo.contextPtr, &retVal, false);

		return retVal.PopPtr();
	}

	return NULL;
}


void CCompositeComponent::OnSubcomponentDeleted(const IComponent* subcomponentPtr)
{
	Q_ASSERT(subcomponentPtr != NULL);

	for (		ComponentMap::iterator iter = m_componentMap.begin();
				iter != m_componentMap.end();
				++iter){
		ComponentInfo& info = iter.value();
		if (info.componentPtr == subcomponentPtr){
			Q_ASSERT(info.isComponentInitialized);

			info.componentPtr->SetComponentContext(NULL, NULL, false);

			info.componentPtr.PopPtr();

			info.isComponentInitialized = false;

			delete this;

			return;
		}
	}

	qFatal("sub component not found in this composition");
}


// reimplemented (icomp::IComponent)

const ICompositeComponent* CCompositeComponent::GetParentComponent(bool ownerOnly) const
{
	if (!ownerOnly || m_isParentOwner){
		return m_parentPtr;
	}

	return NULL;
}


void* CCompositeComponent::GetInterface(const istd::CClassInfo& interfaceType, const QByteArray& subId)
{
	const CCompositeComponentContext* contextPtr = dynamic_cast<const CCompositeComponentContext*>(GetComponentContext());
	if (contextPtr == NULL){
		qCritical("Composite component doesn't use corresponding context");

		return NULL;
	}

	const IRegistry& registry = contextPtr->GetRegistry();

	if (subId.isEmpty()){
		const IRegistry::ExportedInterfacesMap& interfaceInfos = registry.GetExportedInterfacesMap();

		IRegistry::ExportedInterfacesMap::const_iterator iter;
		if (interfaceType.IsVoid()){
			iter = interfaceInfos.constBegin();
		}
		else{
			iter = interfaceInfos.constFind(interfaceType.GetName());
			if ((iter == interfaceInfos.constEnd()) && interfaceType.IsConst()){
				istd::CClassInfo nonConstInterfaceType = interfaceType.GetConstCasted(false);

				iter = interfaceInfos.constFind(nonConstInterfaceType.GetName());
			}
		}

		if (iter != interfaceInfos.constEnd()){
			QByteArray componentId;
			QByteArray restId;
			SplitId(iter.value(), componentId, restId);

			IComponent* subComponentPtr = GetSubcomponent(componentId);
			if (subComponentPtr != NULL){
				return subComponentPtr->GetInterface(interfaceType, restId);
			}
			else{
				qCritical(	"Component %s: Component %s cannot be accessed (interface %s)!",
							contextPtr->GetCompleteContextId().constData(),
							componentId.constData(),
							interfaceType.GetName().constData());
			}
		}
	}
	else{
		QByteArray componentId;
		QByteArray restId;
		SplitId(subId, componentId, restId);

		const IRegistry::ExportedElementsMap& subcomponentMap = registry.GetExportedElementsMap();

		IRegistry::ExportedElementsMap::ConstIterator iter = subcomponentMap.constFind(componentId);
		if (iter != subcomponentMap.constEnd()){
			const QByteArray& realComponentId = iter.value();
			QByteArray subComponentId;
			QByteArray subRestId;
			SplitId(realComponentId, subComponentId, subRestId);

			IComponent* subComponentPtr = GetSubcomponent(subComponentId);
			if (subComponentPtr != NULL){
				return subComponentPtr->GetInterface(interfaceType, JoinId(subRestId, restId));
			}
			else{
				qCritical(	"Component %s: Subcomponent %s is registered as %s, but it cannot be accessed (interface %s)!",
							contextPtr->GetCompleteContextId().constData(),
							subComponentId.constData(),
							componentId.constData(),
							interfaceType.GetName().constData());
			}
		}
		else{
			qCritical(	"Component %s: Subcomponent %s not found!",
						contextPtr->GetCompleteContextId().constData(),
						componentId.constData());
		}
	}

	return NULL;
}


const IComponentContext* CCompositeComponent::GetComponentContext() const
{
	return m_contextPtr;
}


void CCompositeComponent::SetComponentContext(
			const icomp::IComponentContext* contextPtr,
			const ICompositeComponent* parentPtr,
			bool isParentOwner)
{
	m_parentPtr = parentPtr;
	m_isParentOwner = isParentOwner;

	m_autoInitComponentIds.clear();

	const CCompositeComponentContext* compositeContextPtr = dynamic_cast<const CCompositeComponentContext*>(contextPtr);
	if (compositeContextPtr != NULL){
		m_contextPtr = compositeContextPtr;

		const IRegistry& registry = compositeContextPtr->GetRegistry();

		IRegistry::Ids elementIds = registry.GetElementIds();
		for (		IRegistry::Ids::ConstIterator iter = elementIds.constBegin();
					iter != elementIds.constEnd();
					++iter){
			const QByteArray& elementId = *iter;
			const IRegistry::ElementInfo* infoPtr = registry.GetElementInfo(elementId);
			Q_ASSERT(infoPtr);	// ID must be valid, becouse it was taken using icomp::IRegistry::GetElementIds()!

			quint32 flags = infoPtr->elementPtr->GetElementFlags();
			if ((flags & IRegistryElement::EF_AUTO_INSTANCE) != 0){
				m_autoInitComponentIds.insert(elementId);
			}
		}
	}
	else{
		m_contextPtr = NULL;

		for (		ComponentMap::iterator iter = m_componentMap.begin();
					iter != m_componentMap.end();
					++iter){
			ComponentInfo& info = iter.value();

			if (info.isComponentInitialized){
				if (info.componentPtr.IsValid()){
					info.componentPtr->SetComponentContext(NULL, NULL, false);
				}

				info.isComponentInitialized = false;
			}
		}
	}
}


// protected methods

bool CCompositeComponent::CreateSubcomponentInfo(
			const QByteArray& componentId,
			ContextPtr& subContextPtr,
			ComponentPtr* subComponentPtr,
			bool isOwned) const
{
	const CCompositeComponentContext* contextPtr = dynamic_cast<const CCompositeComponentContext*>(GetComponentContext());
	if (contextPtr == NULL){
		return false;
	}

	const IRegistry& registry = contextPtr->GetRegistry();

	const IRegistry::ElementInfo* elementInfoPtr = registry.GetElementInfo(componentId);
	if ((elementInfoPtr != NULL) && elementInfoPtr->elementPtr.IsValid()){
		const CComponentAddress& elementAddress = elementInfoPtr->address;
		const IRegistryElement& registryElement = *elementInfoPtr->elementPtr;

		const IComponentEnvironmentManager& envManager = contextPtr->GetEnvironmentManager();

		const IComponentStaticInfo* subComponentInfoPtr = NULL;
		if (!elementAddress.GetPackageId().isEmpty()){
			subComponentInfoPtr = envManager.GetComponentMetaInfo(elementAddress);
		}
		else{
			const IComponentStaticInfo& info = contextPtr->GetStaticInfo();
			subComponentInfoPtr = info.GetEmbeddedComponentInfo(elementAddress.GetComponentId());
		}

		if (subComponentInfoPtr == NULL){
			return false;
		}

		if (!subContextPtr.IsValid()){
			int componentType = subComponentInfoPtr->GetComponentType();
			if (componentType == IComponentStaticInfo::CT_COMPOSITE){
				// create composed component
				const icomp::IRegistry* subRegistryPtr = envManager.GetRegistry(elementAddress, &registry);
				if (subRegistryPtr != NULL){
					subContextPtr.SetPtr(new CCompositeComponentContext(
								&registryElement,
								subComponentInfoPtr,
								subRegistryPtr,
								&envManager,
								contextPtr,
								componentId));
				}
				else{
					qCritical("Registry %s could not be found", elementAddress.ToString().toLocal8Bit().constData());
				}
			}
			else if (componentType == IComponentStaticInfo::CT_REAL){
				// create real component
				subContextPtr.SetPtr(new CComponentContext(
							&registryElement,
							subComponentInfoPtr,
							contextPtr,
							componentId));
			}
			else{
				qCritical("Unknown component type, instance of %s could not be created. Possible I_BEGIN_COMPONENT macro missing in the component implementation", elementAddress.ToString().toLocal8Bit().constData());
			}
		}

		if (!subContextPtr.IsValid()){
			return false;
		}

		if (subComponentPtr != NULL){
			const IRealComponentStaticInfo* realComponentInfoPtr = dynamic_cast<const IRealComponentStaticInfo*>(subComponentInfoPtr);
			if (realComponentInfoPtr == NULL){
				qCritical("Component %s is not real component, it cannot be created", elementAddress.ToString().toLocal8Bit().constData());

				return false;
			}

			subComponentPtr->SetPtr(realComponentInfoPtr->CreateComponent());
			if (!subComponentPtr->IsValid()){
				qCritical("Instance of component %s couldn't be created", elementAddress.ToString().toLocal8Bit().constData());

				return false;
			}

			(*subComponentPtr)->SetComponentContext(subContextPtr.GetPtr(), this, isOwned);

			if (!m_manualAutoInit || m_autoInitialized){
				icomp::CCompositeComponent* compositeComponentPtr = dynamic_cast<icomp::CCompositeComponent*>((*subComponentPtr).GetPtr());
				if (compositeComponentPtr != NULL){
					compositeComponentPtr->EnsureAutoInitComponentsCreated();
				}
			}
		}
	}

	return true;
}


} // namespace icomp


