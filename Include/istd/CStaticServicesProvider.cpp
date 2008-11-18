#include "istd/CStaticServicesProvider.h"


namespace istd
{


void CStaticServicesProvider::SetParent(const IServicesProvider* parentPtr)
{
	m_parentPtr = parentPtr;
}


bool CStaticServicesProvider::RegisterService(const std::type_info& serviceId, void* servicePtr)
{
	std::string serviceTypeName = std::string(serviceId.name());

	std::pair<Services::iterator, bool> status = m_registeredServices.insert(std::make_pair(serviceTypeName, servicePtr));

	return status.second;
}


void* CStaticServicesProvider::GetService(const std::type_info& serviceId)
{
	Services::const_iterator iter = m_registeredServices.find(std::string(serviceId.name()));

	if (iter != m_registeredServices.end()){
		return iter->second;
	}
	else if(m_parentPtr != NULL){
		return m_parentPtr->GetService(serviceId);
	}

	return NULL;
}


IServicesProvider& CStaticServicesProvider::GetProviderInstance()
{
	return m_providerInstance;
}


// public methods of embedded class IServicesProvider

// reimplemented (istd::IServicesProvider)

void* CStaticServicesProvider::Provider::GetService(const std::type_info& serviceId) const
{
	return CStaticServicesProvider::GetService(serviceId);
}


// static attributes

CStaticServicesProvider::Services CStaticServicesProvider::m_registeredServices;
CStaticServicesProvider::Provider CStaticServicesProvider::m_providerInstance;
const IServicesProvider* CStaticServicesProvider::m_parentPtr(NULL);


} // namespace istd


