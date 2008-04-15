#ifndef icomp_TComponentRegistrator_included
#define icomp_TComponentRegistrator_included


#include <string>

#include "icomp/IComponentStaticInfo.h"
#include "icomp/TBaseComponentStaticInfo.h"


namespace icomp
{


/**
	Simple helper class providing registration of template-specified component in package.
*/
template <class Component>
class TComponentRegistrator
{
public:
	TComponentRegistrator(const std::string& componentId, icomp::IComponentStaticInfo& staticInfo);
};


// public methods

template <class Component>
TComponentRegistrator<Component>::TComponentRegistrator(const std::string& componentId, icomp::IComponentStaticInfo& staticInfo)
{
	static icomp::TComponentStaticInfo<Component> endComponentInfo(&Component::InitStaticInfo(NULL));

	staticInfo.RegisterSubcomponentInfo(componentId, &endComponentInfo);
}


}//namespace icomp


#endif // !icomp_TComponentRegistrator_included


