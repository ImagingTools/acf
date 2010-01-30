#ifndef icomp_CCompositeComponent_included
#define icomp_CCompositeComponent_included


// STL includes
#include <set>
#include <map>
#include <string>

#include "istd/CClassInfo.h"

#include "icomp/IRegistry.h"
#include "icomp/IComponentContext.h"
#include "icomp/CComponentBase.h"
#include "icomp/CInterfaceManipBase.h"


namespace icomp
{


class IMetaInfoManager;


class CCompositeComponent:
			public CComponentBase,
			public CInterfaceManipBase
{
public:
	typedef CComponentBase BaseClass;

	CCompositeComponent();

	template <class InterfaceType>
	InterfaceType* GetComponentInterface(const std::string& subId = "");

	/**
		Begin of blocking of automatic component initialization.
	*/
	void BeginAutoInitBlock();
	/**
		End of blocking of automatic component initialization.
	*/
	bool EndAutoInitBlock();

	// reimplemented (icomp::IComponent)
	virtual void* GetInterface(const istd::CClassInfo& interfaceType, const std::string& subId = "");
	virtual void SetComponentContext(
				const icomp::IComponentContext* contextPtr,
				const IComponent* parentPtr,
				bool isParentOwner);
	virtual IComponent* GetSubcomponent(const std::string& componentId) const;
	virtual const IComponentContext* GetSubcomponentContext(const std::string& componentId) const;
	virtual IComponent* CreateSubcomponent(const std::string& componentId) const;
	virtual void OnSubcomponentDeleted(const IComponent* subcomponentPtr);

protected:
	typedef istd::TDelPtr<icomp::IComponent> ComponentPtr;
	typedef istd::TDelPtr<icomp::IComponentContext> ContextPtr;

	/**
		Create information objects and subcomponent.
		\param	componentId		ID of subcomponent.
		\param	subContextPtr	pointer to subcomponent context will be set to new context object if needed.
		\param	subComponentPtr	optional pointer to subcomponent will be set to new component object.
		\param	isOwned			true, if created component will be owned by this component.
	*/
	void CreateSubcomponentInfo(
				const std::string& componentId,
				ContextPtr& subContextPtr,
				ComponentPtr* subComponentPtr,
				bool isOwned) const;

	bool EnsureAutoInitComponentsCreated() const;

	// reimplemented (icomp::IComponent)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

private:
	struct ComponentInfo
	{
		ComponentInfo(): isInitialized(false){}
		ComponentInfo(const ComponentInfo& info)
		:	componentPtr(info.componentPtr),
			contextPtr(info.contextPtr),
			isInitialized(false){}

		ComponentPtr componentPtr;
		ContextPtr contextPtr;
		bool isInitialized;
	};

	typedef std::map< std::string, ComponentInfo > ComponentMap;

	mutable ComponentMap m_componentMap;

	bool m_blockCreating;

	mutable IRegistry::Ids m_autoInitComponentIds;
	int m_isAutoInitBlockCount;
};


// inline methods

template <class InterfaceType>
inline InterfaceType* CCompositeComponent::GetComponentInterface(const std::string& subId)
{
	static istd::CClassInfo info(typeid(InterfaceType));

	return static_cast<InterfaceType*>(GetInterface(info, subId));
}


} // namespace icomp


#endif // !icomp_CCompositeComponent_included


