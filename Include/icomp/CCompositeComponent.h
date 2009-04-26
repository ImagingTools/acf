#ifndef icomp_CCompositeComponent_included
#define icomp_CCompositeComponent_included


// STL includes
#include <map>
#include <string>

#include "istd/CClassInfo.h"

#include "icomp/IComponentContext.h"
#include "icomp/CComponentBase.h"


namespace icomp
{


class CCompositeComponent: public CComponentBase
{
public:
	typedef CComponentBase BaseClass;

	template <class InterfaceType>
	InterfaceType* GetComponentInterface(const std::string& subId = "");

	CCompositeComponent();

	// reimplemented (icomp::IComponent)
	virtual void* GetInterface(const istd::CClassInfo& interfaceType, const std::string& subId = "");
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();
	virtual IComponent* GetSubcomponent(const std::string& componentId) const;
	virtual IComponent* CreateSubcomponent(const std::string& componentId) const;
	virtual void OnSubcomponentDeleted(const IComponent* subcomponentPtr);

	// static methods
	/**
		Split complete component ID to real component ID and subcomponent ID.
	*/
	static void SplitComponentId(const std::string& fullId, std::string& componentId, std::string& restId);

protected:
	typedef istd::TDelPtr<icomp::IComponent> ComponentPtr;
	typedef istd::TDelPtr<icomp::IComponentContext> ContextPtr;

	/**
		Create information objects and subcomponent.
		\param	componentId		ID of subcomponent.
		\param	subContextPtr	pointer to subcomponent context will be set to new context object if needed.
		\param	subComponentPtr	pointer to subcomponent will be set to new component object.
		\param	isOwned			true, if created component will be owned by this component.
	*/
	bool CreateSubcomponentInfo(
				const std::string& componentId,
				ContextPtr& subContextPtr,
				ComponentPtr& subComponentPtr,
				bool isOwned) const;

	void PrepareToDestroy();

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

	static istd::CClassInfo s_voidType;
};


template <class InterfaceType>
InterfaceType* CCompositeComponent::GetComponentInterface(const std::string& subId)
{
	static istd::CClassInfo info(typeid(InterfaceType));

	return static_cast<InterfaceType*>(GetInterface(info, subId));
}


} // namespace icomp


#endif // !icomp_CCompositeComponent_included


