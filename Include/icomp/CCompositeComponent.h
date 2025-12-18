#ifndef icomp_CCompositeComponent_included
#define icomp_CCompositeComponent_included


// Qt includes
#include <QtCore/QByteArray>
#include <QtCore/QMap>
#include <QtCore/QReadWriteLock>
#include <QtCore/QRecursiveMutex>
#include <QtCore/QWaitCondition>

// ACF includes
#include <istd/CClassInfo.h>
#include <istd/TSmartPtr.h>
#include <icomp/ICompositeComponent.h>
#include <icomp/IRegistry.h>
#include <icomp/CCompositeComponentContext.h>
#include <icomp/CInterfaceManipBase.h>


namespace icomp
{


class IMetaInfoManager;


class CCompositeComponent:
			public CInterfaceManipBase,
			virtual public ICompositeComponent
{
public:
	CCompositeComponent(bool manualAutoInit);
	virtual ~CCompositeComponent();

	/**
		Get interface implemented by this composite component.
	*/
	template <class InterfaceType>
	InterfaceType* GetComponentInterface(const QByteArray& subId = "");

	/**
		Make sure, all components with flag 'AutoInit' are initialized.
		\return	true, if there were components needed to be initialized.
	*/
	bool EnsureAutoInitComponentsCreated() const;

	// reimplemented (icomp::ICompositeComponent)
	virtual IComponentSharedPtr GetSubcomponent(const QByteArray& componentId) const override;
	virtual IComponentContextSharedPtr GetSubcomponentContext(const QByteArray& componentId) const override;
	virtual IComponentUniquePtr CreateSubcomponent(const QByteArray& componentId) const override;
	virtual void OnSubcomponentDeleted(const IComponent* subcomponentPtr) override;

	// reimplemented (icomp::IComponent)
	virtual const icomp::IComponent* GetParentComponent(bool ownerOnly = false) const override;
	virtual void* GetInterface(const istd::CClassInfo& interfaceType, const QByteArray& subId = "") override;
	virtual IComponentContextSharedPtr GetComponentContext() const override;
	virtual void SetComponentContext(
				const IComponentContextSharedPtr& contextPtr,
				const icomp::IComponent* parentPtr,
				bool isParentOwner) override;

protected:
	/**
		Create information objects and subcomponent.
		\param	componentId		ID of subcomponent.
		\param	subContextPtr	pointer to subcomponent context will be set to new context object if needed.
		\param	subComponentPtr	optional pointer to subcomponent will be set to new component object.
		\param	isOwned			true, if created component will be owned by this component and should delegate its destroy event.
		\return					true if success.
	*/
	bool CreateSubcomponentInfo(
				const QByteArray& componentId,
				IComponentContextSharedPtr& subContextPtr,
				IComponentUniquePtr* subComponentPtr,
				bool isOwned) const;

private:
	struct ComponentInfo;

	bool InitializeSubcomponentInfo(
				const QByteArray& componentId,
				ComponentInfo& componentInfo,
				bool isOwned) const;


private:
	enum ComponentState
	{
		CS_NONE,
		CS_INIT,
		CS_READY,
		CS_DESTROYED
	};

	struct ComponentInfo
	{
		ComponentInfo()
			:componentState(CS_NONE),
			isContextInitialized(false)
		{
		}

		/**
			Pointer to component context for some component type.
		*/
		IComponentSharedPtr componentPtr;

		/**
			Flag signaling we tried to create context of this component.
			It is used to avoid multiple initialization of component context.
		*/
		ComponentState componentState = CS_NONE;

		/**
			Pointer to static component for some component type.
		*/
		IComponentContextSharedPtr contextPtr;
		/**
			Flag signaling we tried to create static component.
			It is used to avoid multiple initialization of component.
		*/
		bool isContextInitialized;
	};

	typedef QMap<QByteArray, ComponentInfo> ComponentMap;

	mutable ComponentMap m_componentMap;

	IComponentContextSharedPtr m_contextPtr;
	const icomp::ICompositeComponent* m_parentPtr;
	bool m_isParentOwner;

	bool m_manualAutoInit;

	mutable bool m_autoInitialized;
	mutable IRegistry::Ids m_autoInitComponentIds;

#if QT_VERSION >= 0x060000
	mutable QRecursiveMutex m_mutex;
#else
	mutable QMutex m_mutex;
#endif
};


// inline methods

template <class InterfaceType>
inline InterfaceType* CCompositeComponent::GetComponentInterface(const QByteArray& subId)
{
	static istd::CClassInfo info(typeid(InterfaceType));

	return static_cast<InterfaceType*>(GetInterface(info, subId));
}


} // namespace icomp


#endif // !icomp_CCompositeComponent_included


