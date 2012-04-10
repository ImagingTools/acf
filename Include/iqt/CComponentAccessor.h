#ifndef iqt_CComponentAccessor_included
#define iqt_CComponentAccessor_included


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
	QString m_registryFile;

	icomp::CCompositeComponent m_composite;

	bool m_isAutoInitBlocked;
};


template <class InterfaceType>
InterfaceType* CComponentAccessor::GetComponentInterface(const QByteArray& componentId)
{
	InterfaceType* interfacePtr = m_composite.GetComponentInterface<InterfaceType>(componentId);

	if ((interfacePtr != NULL) && m_isAutoInitBlocked){
		m_composite.EndAutoInitBlock();

		m_isAutoInitBlocked = false;
	}

	return interfacePtr;
}


} // namespace iqt


#endif // !iqt_CComponentAccessor_included


