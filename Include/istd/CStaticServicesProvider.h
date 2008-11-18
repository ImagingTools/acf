#ifndef istd_CStaticServicesProvider_included
#define istd_CStaticServicesProvider_included


#include "istd/istd.h"


#include <map>
#include <string>

#include "istd/IServicesProvider.h"


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
	static void SetParent(const IServicesProvider* parentPtr);
	/**
		Register service for specified ID.
	*/
	static bool RegisterService(const std::type_info& serviceId, void* servicePtr);
	/**
		Register service for specified ID.
	*/
	template <class Service>
	static bool RegisterService(Service* servicePtr);
	static void* GetService(const std::type_info& serviceId);
	static IServicesProvider& GetProviderInstance();

protected:
	class Provider: virtual public IServicesProvider
	{
	public:
		// reimplemented (istd::IServicesProvider)
		virtual void* GetService(const std::type_info& serviceId) const;
	};

private:
	/**
		Lock of constructor.
	*/
	CStaticServicesProvider(){}

	typedef std::map<std::string, void*> Services;

	static Services m_registeredServices;
	static Provider m_providerInstance;
	static const IServicesProvider* m_parentPtr;
};


// public template methods

template <class Service>
bool CStaticServicesProvider::RegisterService(Service* servicePtr)
{
	return RegisterService(typeid(Service), servicePtr);
}


// public template functions

template <typename Service>
Service* GetService()
{
	return static_cast<Service*>(CStaticServicesProvider::GetService(typeid(Service)));
}


} // namespace istd


#endif // !istd_CStaticServicesProvider_included


