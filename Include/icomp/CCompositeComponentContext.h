#ifndef icomp_CCompositeComponentContext_included
#define icomp_CCompositeComponentContext_included


#include "icomp/CComponentContext.h"


namespace icomp
{


class IRegistry;
class IRegistriesManager;
class IComponentEnvironmentManager;


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
		\param	parentPtr				pointer to parent object of this instance.
	*/
	CCompositeComponentContext(
				const IRegistryElement* elementPtr,
				const IComponentStaticInfo* staticInfoPtr,
				const IRegistry* registryPtr,
				const IComponentEnvironmentManager* environmentManagerPtr,
				const CCompositeComponentContext* parentPtr,
				const QByteArray& contextId);

	const IRegistry& GetRegistry() const;
	const IComponentEnvironmentManager& GetEnvironmentManager() const;

private:
	const IRegistry& m_registry;
	const IComponentEnvironmentManager& m_environmentManager;
};


// inline methods

inline const IRegistry& CCompositeComponentContext::GetRegistry() const
{
	return m_registry;
}


inline const IComponentEnvironmentManager& CCompositeComponentContext::GetEnvironmentManager() const
{
	return m_environmentManager;
}


} // namespace icomp


#endif // !icomp_CCompositeComponentContext_included


