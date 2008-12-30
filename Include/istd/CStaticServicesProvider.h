#ifndef istd_CStaticServicesProvider_included
#define istd_CStaticServicesProvider_included


// STL includes
#include <map>
#include <string>

#include "istd/IServicesProvider.h"
#include "istd/CClassInfo.h"


namespace istd
{


/**
	Manage list of global services.
 */
class CStaticServicesProvider
{
public:
	/**
		Set parent provider.
		This provider will be used if this static service provider has no registered service for specified type.
	*/
	static void InitServices(const IServicesProvider* parentPtr);
	/**
		Register service for specified ID.
	*/
	static bool RegisterService(const istd::CClassInfo& serviceId, void* servicePtr);
	/**
		Register service for specified ID.
	*/
	template <class Service>
	static bool RegisterService(Service* servicePtr);
	static void* GetService(const istd::CClassInfo& serviceId);
	static IServicesProvider& GetProviderInstance();

protected:
	class Provider: virtual public IServicesProvider
	{
	public:
		// reimplemented (istd::IServicesProvider)
		virtual void* GetService(const istd::CClassInfo& serviceId) const;
	};

private:
	/**
		Lock of constructor.
	*/
	CStaticServicesProvider(){}

	typedef std::map<istd::CClassInfo, void*> Services;

	static Services s_registeredServices;
	static Provider s_providerInstance;
	static const IServicesProvider* s_parentPtr;
};


// public template methods

template <class Service>
bool CStaticServicesProvider::RegisterService(Service* servicePtr)
{
	static istd::CClassInfo info = istd::CClassInfo::GetInfo<Service>();

	return RegisterService(info, servicePtr);
}


// public template functions

template <typename Service>
Service* GetService()
{
	static istd::CClassInfo info = istd::CClassInfo::GetInfo<Service>();

	return static_cast<Service*>(CStaticServicesProvider::GetService(info));
}


} // namespace istd


#endif // !istd_CStaticServicesProvider_included


