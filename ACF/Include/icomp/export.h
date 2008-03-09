#ifndef icomp_export_included
#define icomp_export_included


#include "icomp/TComponentRegistrator.h"
#include "icomp/CPackageStaticInfo.h"


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


#define I_PACKAGE_EXPORT_FUNCTION GetPackageInfo
#define I_PACKAGE_EXPORT_FUNCTION_NAME "GetPackageInfo"

#define I_EXPORT_PACKAGE(name, description, keywords) \
	static icomp::CPackageStaticInfo packageInfo;\
	extern "C" I_FUNCTION_EXPORT icomp::IComponentStaticInfo* I_PACKAGE_EXPORT_FUNCTION(){\
		return &packageInfo;\
	}

#define I_EXPORT_COMPONENT(component, name, description, keywords) \
	static icomp::TComponentRegistrator<component> name(#name, packageInfo);\

typedef icomp::IComponentStaticInfo* (*GetPackageInfoFunc)();


}//namespace icomp


#endif // !icomp_export_included


