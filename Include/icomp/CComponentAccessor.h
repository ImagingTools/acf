#ifndef icomp_CComponentAccessor_included
#define icomp_CComponentAccessor_included


#include "icomp/IRegistryLoader.h"
#include "icomp/IRegistriesManager.h"
#include "icomp/TComponentWrap.h"
#include "icomp/CCompositeComponentContext.h"
#include "icomp/CCompositeComponent.h"
#include "icomp/CRegistryElement.h"


namespace icomp
{


class CComponentAccessor
{
public:
	explicit CComponentAccessor(
				const istd::CString& registryFile, 
				const istd::CString& configFile = istd::CString());

	template <class InterfaceType>
	InterfaceType* GetComponentInterface(const std::string& componentId = std::string());

private:
	istd::CString m_registryFile;

	icomp::TComponentWrap<icomp::CCompositeComponent> m_composite;
};


template <class InterfaceType>
InterfaceType* CComponentAccessor::GetComponentInterface(const std::string& componentId)
{
	return m_composite.GetComponentInterface<InterfaceType>(componentId);
}


} // namespace icomp


#endif // !icomp_CComponentAccessor_included


