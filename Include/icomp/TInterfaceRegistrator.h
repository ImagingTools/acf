#ifndef icomp_TInterfaceRegistrator_included
#define icomp_TInterfaceRegistrator_included


#include "istd/CClassInfo.h"

#include "icomp/CBaseComponentStaticInfo.h"


namespace icomp
{


/**
	Simple helper class providing registration of template-specified interface.
*/
template <class Interface>
class TInterfaceRegistrator
{
public:
	TInterfaceRegistrator(CBaseComponentStaticInfo& staticInfo);

protected:
	static void* InterfaceExtractor(IComponent& component);
};


// public methods

template <class Interface>
TInterfaceRegistrator<Interface>::TInterfaceRegistrator(CBaseComponentStaticInfo& staticInfo)
{
	istd::CClassInfo info = istd::CClassInfo::GetInfo<Interface>();

	if (!info.IsVoid()){
		staticInfo.RegisterInterfaceExtractor(info.GetName(), TInterfaceRegistrator<Interface>::InterfaceExtractor);
	}
}


template <>
inline TInterfaceRegistrator<void>::TInterfaceRegistrator(CBaseComponentStaticInfo& /*staticInfo*/)
{
}


// protected methods

template <class Interface>
void* TInterfaceRegistrator<Interface>::InterfaceExtractor(IComponent& component)
{
	void* retVal = dynamic_cast<Interface*>(&component);

	return retVal;
}


template <>
inline void* TInterfaceRegistrator<void>::InterfaceExtractor(IComponent& /*component*/)
{
	return NULL;
}


}//namespace icomp


#endif // !icomp_TInterfaceRegistrator_included


