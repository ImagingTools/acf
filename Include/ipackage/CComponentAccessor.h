#ifndef ipackage_CComponentAccessor_included
#define ipackage_CComponentAccessor_included


// ACF includes
#include "istd/TDelPtr.h"
#include "icomp/IComponentStaticInfo.h"
#include "icomp/IComponentContext.h"
#include "icomp/CCompositeComponent.h"


namespace ipackage
{

	
/**
	A helper class to access public interfaces of a composite component.
*/
class CComponentAccessor
{
public:
	/**
		Constructor.
		
		\param	registryFile	The registry file.
		\param	configFile  	(Optional) the configuration file.
	*/
	explicit CComponentAccessor(
				const QString& registryFile, 
				const QString& configFile = QString(),
				bool isDiagnosticEnabled = false);

	/**
		Gets component interface.
	
		\tparam	InterfaceType	Type of the interface.
		\param	componentId	(Optional) identifier for the component.
		
		\return	null if it fails, else the requested component interface.
	*/
	template <class InterfaceType>
	InterfaceType* GetComponentInterface(const QByteArray& componentId = QByteArray());

private:
	icomp::CCompositeComponent m_mainComponent;

	istd::TDelPtr<icomp::IComponentStaticInfo> m_mainComponentStaticInfoPtr;
	istd::TDelPtr<icomp::IComponentContext> m_mainComponentContextPtr;

	bool m_isAutoInitBlocked;
};


template <class InterfaceType>
InterfaceType* CComponentAccessor::GetComponentInterface(const QByteArray& componentId)
{
	InterfaceType* interfacePtr = m_mainComponent.GetComponentInterface<InterfaceType>(componentId);

	return interfacePtr;
}


} // namespace ipackage


#endif // !ipackage_CComponentAccessor_included


