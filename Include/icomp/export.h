#ifndef icomp_export_included
#define icomp_export_included


#include "icomp/TComponentRegistrator.h"
#include "icomp/CPackageStaticInfo.h"

#include "istd/CStaticServicesProvider.h"


namespace icomp
{


#ifdef WIN32
	#define I_FUNCTION_EXPORT _declspec(dllexport)
#else
	#ifdef LINUX
		#define I_FUNCTION_EXPORT
	#else
		#define I_FUNCTION_EXPORT
	#endif
#endif

#ifdef _DEBUG
	#define I_PACKAGE_EXPORT_FUNCTION GetPackageInfoDebug
	#define I_PACKAGE_EXPORT_FUNCTION_NAME "GetPackageInfoDebug"
#else
	#define I_PACKAGE_EXPORT_FUNCTION GetPackageInfo
	#define I_PACKAGE_EXPORT_FUNCTION_NAME "GetPackageInfo"
#endif


#ifdef _DEBUG
	#define I_EXPORT_SERVICES_FUNCTION RegisterServicesDebug
	#define I_EXPORT_SERVICES_FUNCTION_NAME "RegisterServicesDebug"
#else
	#define I_EXPORT_SERVICES_FUNCTION RegisterServices
	#define I_EXPORT_SERVICES_FUNCTION_NAME "RegisterServices"
#endif


#define I_EXPORT_SERVICES_PROVIDER(serviceProvider)\
	extern "C" I_FUNCTION_EXPORT void I_EXPORT_SERVICES_FUNCTION(const istd::IServicesProvider* parentPtr){\
		istd::CStaticServicesProvider::SetParent(parentPtr);\
		serviceProvider::RegisterServices();\
	}

#define I_EXPORT_DEFAULT_SERVICES\
	extern "C" I_FUNCTION_EXPORT void I_EXPORT_SERVICES_FUNCTION(const istd::IServicesProvider* parentPtr){\
		istd::CStaticServicesProvider::SetParent(parentPtr);\
	}


#define I_EXPORT_PACKAGE(name, description, keywords) \
	static icomp::CPackageStaticInfo packageInfo;\
	extern "C" I_FUNCTION_EXPORT icomp::IComponentStaticInfo* I_PACKAGE_EXPORT_FUNCTION(){\
		return &packageInfo;\
	}

#define I_EXPORT_COMPONENT(component, description, keywords) \
	static icomp::TComponentRegistrator< component > component##_registrator(#component, packageInfo, description, keywords);\

typedef icomp::IComponentStaticInfo* (*GetPackageInfoFunc)();
typedef void (*RegisterServicesFunc)(const istd::IServicesProvider*);


} // namespace icomp


#endif // !icomp_export_included


