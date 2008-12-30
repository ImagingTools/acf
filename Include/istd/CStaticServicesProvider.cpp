#include "istd/CStaticServicesProvider.h"


namespace istd
{


void CStaticServicesProvider::InitServices(const IServicesProvider* parentPtr)
{
	if (s_parentPtr == NULL){
		s_parentPtr = parentPtr;
	}
}


bool CStaticServicesProvider::RegisterService(const istd::CClassInfo& serviceId, void* servicePtr)
{
	std::pair<Services::iterator, bool> status = s_registeredServices.insert(std::make_pair(serviceId, servicePtr));

	return status.second;
}


void* CStaticServicesProvider::GetService(const istd::CClassInfo& serviceId)
{
	Services::const_iterator iter = s_registeredServices.find(serviceId);

	if (iter != s_registeredServices.end()){
		return iter->second;
	}
	else if(s_parentPtr != NULL){
		return s_parentPtr->GetService(serviceId);
	}

	return NULL;
}


IServicesProvider& CStaticServicesProvider::GetProviderInstance()
{
	return s_providerInstance;
}


// public methods of embedded class IServicesProvider

// reimplemented (istd::IServicesProvider)

void* CStaticServicesProvider::Provider::GetService(const istd::CClassInfo& serviceId) const
{
	return CStaticServicesProvider::GetService(serviceId);
}


// static attributes

CStaticServicesProvider::Services CStaticServicesProvider::s_registeredServices;
CStaticServicesProvider::Provider CStaticServicesProvider::s_providerInstance;
const IServicesProvider* CStaticServicesProvider::s_parentPtr(NULL);


} // namespace istd


