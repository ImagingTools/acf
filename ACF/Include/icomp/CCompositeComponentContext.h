#ifndef icomp_CCompositeComponentContext_included
#define icomp_CCompositeComponentContext_included


#include "icomp/CComponentContext.h"


namespace icomp
{


class IRegistry;
class IRegistriesManager;


class CCompositeComponentContext: public CComponentContext
{
public:
	typedef CComponentContext BaseClass;

	/**
		Constructor.
		\param	elementPtr				registry element object describing parameterisation of this composition treated as component.
										It can not be NULL.
		\param	registryPtr				data model object used by this composition.
										It can not be NULL.
		\param	registriesManagerPtr	registry manager allowing to access to registries if composite subelement is created.
		\param	componentFactoryPtr		root object of native components factory.
										Subelements of this object are instances of CPackageStaticInfo allowing access to components factory.
										It can not be NULL.
		\param	parentPtr				pointer to parent object of this instance.
	*/
	CCompositeComponentContext(
				const IRegistryElement* elementPtr,
				const IRegistry* registryPtr,
				const IRegistriesManager* registriesManagerPtr,
				const CCompositeComponentContext* parentPtr = NULL);

	const IRegistry& GetRegistry() const;
	const IRegistriesManager& GetRegistriesManager() const;

	// reimplemented (icomp::IComponentContext)
	virtual IComponent* GetSubcomponent(const std::string& componentId) const;
	virtual IComponent* CreateSubcomponent(const std::string& componentId) const;

protected:
	typedef istd::TDelPtr<icomp::IComponent> ComponentPtr;
	typedef istd::TDelPtr<icomp::IComponentContext> ContextPtr;

	bool CreateSubcomponentInfo(const std::string& componentId, ContextPtr& contextPtr, ComponentPtr& result) const;

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

	const IRegistry& m_registry;
	const IRegistriesManager& m_registriesManager;
};


// inline methods

inline const IRegistry& CCompositeComponentContext::GetRegistry() const
{
	return m_registry;
}


inline const IRegistriesManager& CCompositeComponentContext::GetRegistriesManager() const
{
	return m_registriesManager;
}


}//namespace icomp


#endif // !icomp_CCompositeComponentContext_included


