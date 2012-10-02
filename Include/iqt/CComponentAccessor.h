#ifndef iqt_CComponentAccessor_included
#define iqt_CComponentAccessor_included


// ACF includes
#include "istd/TDelPtr.h"
#include "icomp/IComponentStaticInfo.h"
#include "icomp/IComponentContext.h"
#include "icomp/CCompositeComponent.h"


namespace iqt
{


class CComponentAccessor
{
public:
	explicit CComponentAccessor(
				const QString& registryFile, 
				const QString& configFile = QString());

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

	if ((interfacePtr != NULL) && m_isAutoInitBlocked){
		m_mainComponent.EndAutoInitBlock();

		m_isAutoInitBlocked = false;
	}

	return interfacePtr;
}


} // namespace iqt


#endif // !iqt_CComponentAccessor_included


