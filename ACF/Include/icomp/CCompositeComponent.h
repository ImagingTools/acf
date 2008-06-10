#ifndef icomp_CCompositeComponent_included
#define icomp_CCompositeComponent_included


#include <string>

#include "icomp/CComponentBase.h"


namespace icomp
{


class CCompositeComponent: virtual public CComponentBase
{
public:
	template <class InterfaceType>
	InterfaceType* GetComponentInterface(const std::string& subId = "");

	// reimplemented (icomp::IComponent)
	virtual void* GetInterface(const type_info& interfaceType, const std::string& subId = "");

protected:
	/**
		Split complete component ID to real component ID and subcomponent ID.
	*/
	static void SplitComponentId(const std::string& fullId, std::string& componentId, std::string& restId);
};


template <class InterfaceType>
InterfaceType* CCompositeComponent::GetComponentInterface(const std::string& subId)
{
	return static_cast<InterfaceType*>(GetInterface(typeid(InterfaceType),subId));
}


} // namespace icomp


#endif // !icomp_CCompositeComponent_included


