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
	:m_contextPtr(nullptr),
	m_parentPtr(nullptr),
	m_isParentOwner(false),
	m_manualAutoInit(manualAutoInit),
	m_autoInitialized(false)
#if QT_VERSION < 0x060000
	,m_mutex(QMutex::Recursive)
#endif
{
}


CCompositeComponent::~CCompositeComponent()
{
	QMutexLocker lock(&m_mutex);

	if (m_isParentOwner && (m_parentPtr != nullptr)){
		const_cast<ICompositeComponent*>(m_parentPtr)->OnSubcomponentDeleted(this);
	}

	SetComponentContext(IComponentContextSharedPtr(), nullptr, false);

	m_componentMap.clear();
}


bool CCompositeComponent::EnsureAutoInitComponentsCreated() const
{
	QMutexLocker writeLock(&m_mutex);

	bool retVal = false;

	if ((m_contextPtr != nullptr) && !m_autoInitialized){
		m_autoInitialized = true;

		while (!m_autoInitComponentIds.isEmpty()){
			QByteArray autoInitId = *m_autoInitComponentIds.begin();

			m_autoInitComponentIds.erase(m_autoInitComponentIds.begin());

			ComponentInfo& autoInitInfo = m_componentMap[autoInitId];
			if (autoInitInfo.componentState == CS_NONE){
				autoInitInfo.componentState = CS_INIT;
				autoInitInfo.isContextInitialized = true;

				InitializeSubcomponentInfo(autoInitId, autoInitInfo, true);

				retVal = true;
			}
		}
		
		QList<icomp::CCompositeComponent*> subComponentsList;

		for (		ComponentMap::iterator iter = m_componentMap.begin();
					iter != m_componentMap.end();
					++iter){
			ComponentInfo& info = iter.value();
			icomp::CCompositeComponent* compositeComponentPtr = dynamic_cast<icomp::CCompositeComponent*>(info.componentPtr.get());
			if (compositeComponentPtr != NULL){
				subComponentsList.push_back(compositeComponentPtr);
			}
		}

		for (		QList<icomp::CCompositeComponent*>::const_iterator iter = subComponentsList.cbegin();
					iter != subComponentsList.cend();
					++iter){
			retVal = (*iter)->EnsureAutoInitComponentsCreated() || retVal;
		}
	}

	return retVal;
}


// reimplemented (icomp::ICompositeComponent)

IComponentSharedPtr CCompositeComponent::GetSubcomponent(const QByteArray& componentId) const
{
	QMutexLocker lock(&m_mutex);

	if (m_contextPtr != nullptr){
		ComponentMap::ConstIterator iter = m_componentMap.constFind(componentId);
		if (iter == m_componentMap.constEnd()){
			ComponentInfo& componentInfo = m_componentMap[componentId];
			componentInfo.componentState = CS_INIT;

			componentInfo.isContextInitialized = true;
 
			InitializeSubcomponentInfo(componentId, componentInfo, true);

			return componentInfo.componentPtr;
		}
		else {
			ComponentInfo& componentInfo = m_componentMap[componentId];
			if (componentInfo.componentState < CS_READY){
				componentInfo.componentState = CS_INIT;

				componentInfo.isContextInitialized = true;

				InitializeSubcomponentInfo(componentId, componentInfo, true);
			}

			return componentInfo.componentPtr;
		}
	}
	else{
		ComponentMap::ConstIterator iter = m_componentMap.constFind(componentId);
		if (iter != m_componentMap.constEnd()){
			return iter.value().componentPtr;
		}
		else{
			return IComponentSharedPtr();
		}
	}

	return IComponentSharedPtr();
}


IComponentContextSharedPtr CCompositeComponent::GetSubcomponentContext(const QByteArray& componentId) const
{
	QMutexLocker lock(&m_mutex);

	if (m_contextPtr != nullptr){
		ComponentInfo& componentInfo = m_componentMap[componentId];
		if (!componentInfo.isContextInitialized){
			componentInfo.isContextInitialized = true;

			CreateSubcomponentInfo(componentId, componentInfo.contextPtr, NULL, true);
		}

		return componentInfo.contextPtr;
	}
	else{
		ComponentMap::ConstIterator iter = m_componentMap.constFind(componentId);
		if (iter != m_componentMap.constEnd()){
			return iter.value().contextPtr;
		}
		else{
			return IComponentContextSharedPtr();
		}
	}
}


IComponentUniquePtr CCompositeComponent::CreateSubcomponent(const QByteArray& componentId) const
{
	QMutexLocker lock(&m_mutex);

	if (m_contextPtr != nullptr){
		ComponentInfo& componentInfo = m_componentMap[componentId];

		componentInfo.isContextInitialized = true;

		IComponentUniquePtr retVal;
		CreateSubcomponentInfo(componentId, componentInfo.contextPtr, &retVal, false);

		return retVal;
	}

	return IComponentUniquePtr();
}


void CCompositeComponent::OnSubcomponentDeleted(const IComponent* subcomponentPtr)
{
	Q_ASSERT(subcomponentPtr != NULL);

	QMutexLocker lock(&m_mutex);

	for (		ComponentMap::iterator iter = m_componentMap.begin();
				iter != m_componentMap.end();
				++iter){
		ComponentInfo& info = iter.value();
		if (info.componentPtr.get() == subcomponentPtr){
			Q_ASSERT(info.componentState == CS_READY);

			info.componentPtr->SetComponentContext(IComponentContextSharedPtr(), nullptr, false);

			info.componentPtr.reset();

			info.componentState = CS_DESTROYED;

			return;
		}
	}

	qFatal("sub component not found in this composition");
}


// reimplemented (icomp::IComponent)

const icomp::IComponent* CCompositeComponent::GetParentComponent(bool ownerOnly) const
{
	if (!ownerOnly || m_isParentOwner){
		return m_parentPtr;
	}

	return nullptr;
}


void* CCompositeComponent::GetInterface(const istd::CClassInfo& interfaceType, const QByteArray& subId)
{
	const CCompositeComponentContext* contextPtr = dynamic_cast<const CCompositeComponentContext*>(GetComponentContext().get());
	if (contextPtr == NULL){
		qCritical("Composite component doesn't use corresponding context");

		return NULL;
	}

	const IRegistry& registry = contextPtr->GetRegistry();

	if (subId.isEmpty()){
		// Make a copy to avoid holding the lock during iteration
		const IRegistry::ExportedInterfacesMap interfaceInfos = registry.GetExportedInterfacesMap();

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

			IComponentSharedPtr subComponentPtr = GetSubcomponent(componentId);
			if (subComponentPtr != nullptr){
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

		// Make a copy to avoid holding the lock during iteration
		const IRegistry::ExportedElementsMap subcomponentMap = registry.GetExportedElementsMap();

		IRegistry::ExportedElementsMap::ConstIterator iter = subcomponentMap.constFind(componentId);
		if (iter != subcomponentMap.constEnd()){
			const QByteArray& realComponentId = iter.value();
			QByteArray subComponentId;
			QByteArray subRestId;
			SplitId(realComponentId, subComponentId, subRestId);

			IComponentSharedPtr subComponentPtr = GetSubcomponent(subComponentId);
			if (subComponentPtr != nullptr){
				return subComponentPtr->GetInterface(interfaceType, JoinId(subRestId, restId));
			}
			else{
				qCritical("Component %s: Subcomponent %s is registered as %s, but it cannot be accessed (interface %s)!",
							contextPtr->GetCompleteContextId().constData(),
							subComponentId.constData(),
							componentId.constData(),
							interfaceType.GetName().constData());
			}
		}
		else{
			qCritical("Component %s: Subcomponent %s not found!",
						contextPtr->GetCompleteContextId().constData(),
						componentId.constData());
		}
	}

	return NULL;
}


IComponentContextSharedPtr CCompositeComponent::GetComponentContext() const
{
	return m_contextPtr;
}


void CCompositeComponent::SetComponentContext(
			const IComponentContextSharedPtr& contextPtr,
			const icomp::IComponent* parentPtr,
			bool isParentOwner)
{
	QMutexLocker lock(&m_mutex);

	const icomp::ICompositeComponent* compositeParentPtr = dynamic_cast<const icomp::ICompositeComponent*>(parentPtr);

	m_parentPtr = compositeParentPtr;
	m_isParentOwner = isParentOwner;

	m_autoInitComponentIds.clear();

	const CCompositeComponentContext* compositeContextPtr = dynamic_cast<const CCompositeComponentContext*>(contextPtr.get());
	if (compositeContextPtr != NULL){
		m_contextPtr = contextPtr;

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
		m_contextPtr.reset();

		for (		ComponentMap::iterator iter = m_componentMap.begin();
					iter != m_componentMap.end();
					++iter){
			ComponentInfo& info = iter.value();

			if (info.componentState == CS_READY){
				if (info.componentPtr != nullptr){
					info.componentPtr->SetComponentContext(IComponentContextSharedPtr(), nullptr, false);
				}

				info.componentState = CS_DESTROYED;
			}
		}
	}
}


// protected methods

bool CCompositeComponent::CreateSubcomponentInfo(
			const QByteArray& componentId,
			IComponentContextSharedPtr& subContextPtr,
			IComponentUniquePtr* subComponentPtr,
			bool isOwned) const
{
	const CCompositeComponentContext* contextPtr = dynamic_cast<const CCompositeComponentContext*>(GetComponentContext().get());
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

		if (subContextPtr == nullptr){
			int componentType = subComponentInfoPtr->GetComponentType();
			if (componentType == IComponentStaticInfo::CT_COMPOSITE){
				// create composed component
				const icomp::IRegistry* subRegistryPtr = envManager.GetRegistry(elementAddress, &registry);
				if (subRegistryPtr != NULL){
					subContextPtr.reset(new CCompositeComponentContext(
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
				subContextPtr.reset(new CComponentContext(
							&registryElement,
							subComponentInfoPtr,
							contextPtr,
							componentId));
			}
			else{
				qCritical("Unknown component type, instance of %s could not be created. Possible I_BEGIN_COMPONENT macro missing in the component implementation", elementAddress.ToString().toLocal8Bit().constData());
			}
		}

		if (subContextPtr == nullptr){
			return false;
		}

		if (subComponentPtr != nullptr){
			const IRealComponentStaticInfo* realComponentInfoPtr = dynamic_cast<const IRealComponentStaticInfo*>(subComponentInfoPtr);
			if (realComponentInfoPtr == NULL){
				qCritical("Component %s is not real component, it cannot be created", elementAddress.ToString().toLocal8Bit().constData());

				return false;
			}

			*subComponentPtr = std::move(realComponentInfoPtr->CreateComponent());
			if (*subComponentPtr == nullptr){
				qCritical("Instance of component %s couldn't be created", elementAddress.ToString().toLocal8Bit().constData());

				return false;
			}

			(*subComponentPtr)->SetComponentContext(subContextPtr, this, isOwned);

			if (!m_manualAutoInit || m_autoInitialized){
				icomp::CCompositeComponent* compositeComponentPtr = dynamic_cast<icomp::CCompositeComponent*>((*subComponentPtr).get());
				if (compositeComponentPtr != NULL){
					compositeComponentPtr->EnsureAutoInitComponentsCreated();
				}
			}
		}
	}

	return true;
}


bool CCompositeComponent::InitializeSubcomponentInfo(
			const QByteArray& componentId,
			ComponentInfo& componentInfo,
			bool isOwned) const
{
	IComponentContextSharedPtr myContextPtr = GetComponentContext();
	if (myContextPtr == nullptr) {
		qCritical("No context defined");

		return false;
	}

	const CCompositeComponentContext* contextImplPtr = dynamic_cast<const CCompositeComponentContext*>(myContextPtr.get());
	if (contextImplPtr == nullptr){
		qCritical("Wrong context type");

		return false;
	}

	const IRegistry& registry = contextImplPtr->GetRegistry();

	const IRegistry::ElementInfo* elementInfoPtr = registry.GetElementInfo(componentId);
	if ((elementInfoPtr != NULL) && elementInfoPtr->elementPtr.IsValid()){
		const CComponentAddress& elementAddress = elementInfoPtr->address;
		const IRegistryElement& registryElement = *elementInfoPtr->elementPtr;

		const IComponentEnvironmentManager& envManager = contextImplPtr->GetEnvironmentManager();

		const IComponentStaticInfo* subComponentInfoPtr = nullptr;
		if (!elementAddress.GetPackageId().isEmpty()){
			subComponentInfoPtr = envManager.GetComponentMetaInfo(elementAddress);
		}
		else {
			const IComponentStaticInfo& info = contextImplPtr->GetStaticInfo();
			subComponentInfoPtr = info.GetEmbeddedComponentInfo(elementAddress.GetComponentId());
		}

		if (subComponentInfoPtr == nullptr){
			qCritical("Sub-component information was not found in the registry");

			return false;
		}

		if (componentInfo.contextPtr == nullptr){
			int componentType = subComponentInfoPtr->GetComponentType();
			if (componentType == IComponentStaticInfo::CT_COMPOSITE){
				// create composed component
				const icomp::IRegistry* subRegistryPtr = envManager.GetRegistry(elementAddress, &registry);
				if (subRegistryPtr != NULL){
					componentInfo.contextPtr.reset(new CCompositeComponentContext(
						&registryElement,
						subComponentInfoPtr,
						subRegistryPtr,
						&envManager,
						contextImplPtr,
						componentId));
				}
				else {
					qCritical("Registry %s could not be found", elementAddress.ToString().toLocal8Bit().constData());
				}
			}
			else if (componentType == IComponentStaticInfo::CT_REAL){
				// create real component
				componentInfo.contextPtr.reset(new CComponentContext(
					&registryElement,
					subComponentInfoPtr,
					contextImplPtr,
					componentId));
			}
			else {
				qCritical("Unknown component type, instance of %s could not be created. Possible I_BEGIN_COMPONENT macro missing in the component implementation", elementAddress.ToString().toLocal8Bit().constData());
			}
		}

		if (componentInfo.contextPtr == nullptr){
			qCritical("Context could not be set for the target component");

			return false;
		}

		const IRealComponentStaticInfo* realComponentInfoPtr = dynamic_cast<const IRealComponentStaticInfo*>(subComponentInfoPtr);
		if (realComponentInfoPtr == NULL){
			qCritical("Component %s is not real component, it cannot be created", elementAddress.ToString().toLocal8Bit().constData());

			return false;
		}

		IComponentUniquePtr realComponentPtr = realComponentInfoPtr->CreateComponent();

		componentInfo.componentPtr = std::move(realComponentPtr);
		if (componentInfo.componentPtr == nullptr){
			qCritical("Instance of component %s couldn't be created", elementAddress.ToString().toLocal8Bit().constData());

			return false;
		}

		componentInfo.componentState = CS_READY;

		componentInfo.componentPtr->SetComponentContext(componentInfo.contextPtr, this, isOwned);

		if (!m_manualAutoInit || m_autoInitialized){
			icomp::CCompositeComponent* compositeComponentPtr = dynamic_cast<icomp::CCompositeComponent*>(componentInfo.componentPtr.get());
			if (compositeComponentPtr != NULL){
				compositeComponentPtr->EnsureAutoInitComponentsCreated();
			}
		}
	}

	return true;
}


} // namespace icomp


