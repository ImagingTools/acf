#ifndef icomp_export_included
#define icomp_export_included


#include "icomp/TComponentRegistrator.h"
#include "icomp/CPackageStaticInfo.h"

#include "istd/CStaticServicesProvider.h"


namespace icomp
{


#if defined _MSC_VER
	#define I_FUNCTION_EXPORT __declspec(dllexport) // Note: actually gcc seems to also supports this syntax.
#elif defined __GNUC__
	#define I_FUNCTION_EXPORT __attribute__ ((visibility("default")))
#else
	#define I_FUNCTION_EXPORT
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
		istd::CStaticServicesProvider::InitServices(parentPtr);\
		serviceProvider::RegisterServices();\
	}

#define I_EXPORT_DEFAULT_SERVICES\
	extern "C" I_FUNCTION_EXPORT void I_EXPORT_SERVICES_FUNCTION(const istd::IServicesProvider* parentPtr){\
		istd::CStaticServicesProvider::InitServices(parentPtr);\
	}


#define I_EXPORT_PACKAGE(logicalName, description, keywords) \
	static icomp::CPackageStaticInfo packageInfo(description, keywords);\
	extern "C" I_FUNCTION_EXPORT icomp::IComponentStaticInfo* I_PACKAGE_EXPORT_FUNCTION(){\
		return &packageInfo;\
	}

#define I_EXPORT_COMPONENT(component, description, keywords) \
	static icomp::TComponentRegistrator<component> component##_registrator(#component, packageInfo, description, keywords);\

typedef icomp::IComponentStaticInfo* (*GetPackageInfoFunc)();
typedef void (*RegisterServicesFunc)(const istd::IServicesProvider*);


#define I_DATA_PERSISTENCE "\"Data Persistence\""
#define I_DATA_PROCESSING "\"Data Processing\""
#define I_SERVICE "Service"
#define I_APPLICATION "Application"
#define I_APPLICATION_LOGIC "\"Application Logic\""
#define I_DATA_MODEL "\"Data Model\""
#define I_GUI "GUI"
#define I_DATA_VISUALIZATION "Visualization"
#define I_PACKAGE "Package"

// defines used to simplify using of meta description
#define IM_CATEGORY(keywords) " Category='" ##keywords "' "
#define IM_TAG(keywords) " Tag='" ##keywords "' "
#define IM_COMPANY(keywords) " Company='" ##keywords "' "
#define IM_AUTHOR(keywords) " Author='" ##keywords "' "
#define IM_PROJECT(keywords) " Project='" ##keywords "' "


} // namespace icomp


#endif // !icomp_export_included


