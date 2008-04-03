#include "istd/CStaticServicesProvider.h"


namespace istd
{


void CStaticServicesProvider::SetParent(const IServicesProvider* parentPtr)
{
	m_parentPtr = parentPtr;
}


bool CStaticServicesProvider::RegisterService(const type_info& serviceId, void* servicePtr)
{
	std::pair<Services::iterator, bool> status = m_registeredServices.insert(Services::value_type(&serviceId, servicePtr));

	return status.second;
}


void* CStaticServicesProvider::GetService(const type_info& serviceId)
{
	Services::const_iterator iter = m_registeredServices.find(&serviceId);

	if (iter != m_registeredServices.end()){
		return iter->second;
	}

	return NULL;
}


IServicesProvider& CStaticServicesProvider::GetProviderInstance()
{
	return m_providerInstance;
}


// public methods of embedded class IServicesProvider

// reimplemented (istd::IServicesProvider)

void* CStaticServicesProvider::Provider::GetService(const type_info& serviceId) const
{
	return CStaticServicesProvider::GetService(serviceId);
}


// static attributes

CStaticServicesProvider::Services CStaticServicesProvider::m_registeredServices;
CStaticServicesProvider::Provider CStaticServicesProvider::m_providerInstance;
const IServicesProvider* CStaticServicesProvider::m_parentPtr(NULL);


} // namespace istd


