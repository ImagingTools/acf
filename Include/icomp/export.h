#ifndef icomp_export_included
#define icomp_export_included


// Qt includes
#include <QtCore/QtGlobal>

// ACF includes
#include <icomp/TComponentRegistrator.h>
#include <icomp/CPackageStaticInfo.h>


namespace icomp
{


#if defined _MSC_VER || defined __MINGW32__ || defined __MINGW64__
	#define I_FUNCTION_EXPORT __declspec(dllexport) // Note: actually gcc seems also to supports this syntax.
#elif defined __GNUC__
	#define I_FUNCTION_EXPORT __attribute__ ((visibility("default")))
#else
	#define I_FUNCTION_EXPORT
#endif


#ifndef QT_NO_DEBUG
	#define I_PACKAGE_EXPORT_FUNCTION GetPackageInfoDebug
	#define I_PACKAGE_EXPORT_FUNCTION_NAME "GetPackageInfoDebug"
#else // !QT_NO_DEBUG
	#define I_PACKAGE_EXPORT_FUNCTION GetPackageInfo
	#define I_PACKAGE_EXPORT_FUNCTION_NAME "GetPackageInfo"
#endif // !QT_NO_DEBUG


#define I_EXPORT_PACKAGE(logicalName, description, keywords) \
	static icomp::CPackageStaticInfo packageInfo(description, keywords);\
	extern "C" I_FUNCTION_EXPORT icomp::CPackageStaticInfo* I_PACKAGE_EXPORT_FUNCTION(){\
		return &packageInfo;\
	}

#define I_EXPORT_COMPONENT(component, description, keywords) \
	static icomp::TComponentRegistrator<component> component##_registrator(#component, packageInfo, description, keywords);\

typedef icomp::CPackageStaticInfo* (*GetPackageInfoFunc)();


#define I_DATA_PERSISTENCE "\"Data Persistence\""
#define I_DATA_PROCESSING "\"Data Processing\""
#define I_SERVICE "Service"
#define I_APPLICATION "Application"
#define I_APPLICATION_LOGIC "\"Application Logic\""
#define I_DATA_MODEL "\"Data Model\""
#define I_GUI "GUI"
#define I_DATA_VISUALIZATION "Visualization"
#define I_PACKAGE "Package"

// Macros used to simplify using of meta description
#define IM_CATEGORY(keywords) " Category='" keywords "' "
#define IM_TAG(keywords) " Tag='" keywords "' "
#define IM_COMPANY(keywords) " Company='" keywords "' "
#define IM_AUTHOR(keywords) " Author='" keywords "' "
#define IM_PROJECT(keywords) " Project='" keywords "' "

#define I_REGISTER_QT_RESOURCE(resourceId)\
static struct Registrator_##resourceId\
{\
	Registrator_##resourceId()\
	{\
		Q_INIT_RESOURCE(resourceId);\
	}\
} registrator_instance_##resourceId;\


} // namespace icomp


#endif // !icomp_export_included


